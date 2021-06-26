import kivy
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.widget import Widget
from kivy.core.window import Window
from kivy.uix.actionbar import ActionButton
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from serial.serialutil import EIGHTBITS, PARITY_EVEN, STOPBITS_ONE, SerialException
from serial.serialwin32 import Serial
from serial.tools.list_ports import comports
import serial
from serial import Serial
from serial.tools import list_ports
import threading
import time
from kivy.uix.textinput import TextInput
from kivy.uix.image import Image
import numpy as np
import open3d as o3d
import os
import datetime
import io

from kivy.graphics import Rectangle, Color


class PointCloud:
    def __init__(self, coordinates):
        self.pcd = o3d.geometry.PointCloud()
        self.pcd.points = o3d.utility.Vector3dVector(coordinates[:, :3])
        self.poisson_mesh = None
        self.ballpoint_mesh = None
        self.tetra_mesh = None
        self.pt_map = None

    @classmethod
    def with_normals(cls, coordinates, normals):
        pcd = cls(coordinates)
        pcd.add_normals(normals)
        return pcd

    @classmethod
    def with_colors(cls, coordinates, colors):
        pcd = cls(coordinates)
        pcd.add_colors(colors)

    @classmethod
    def with_colors_and_normals(cls, coordinates, normals, colors):
        pcd = cls(coordinates)
        pcd.add_normals(normals)
        pcd.add_colors(colors)

    def add_normals(self, normals):
        self.pcd.normals = o3d.utility.Vector3dVector(normals[:, :3])

    def add_colors(self, colors):
        self.pcd.colors = o3d.utility.Vector3dVector(colors[:, :3])

    @staticmethod
    def visualize(mesh):
        vis = o3d.visualization.Visualizer()
        vis.create_window()
        vis.add_geometry(mesh)
        ctr = vis.get_view_control()
        ctr.set_zoom(0.45)
        vis.run()
        vis.destroy_window()

    @staticmethod
    def write(mesh, filename="mesh", output_folder="mesh"):
        os.makedirs(output_folder, exist_ok=True)
        now = datetime.datetime.now()
        filename = f"{filename}_{now.strftime('%Y_%m_%dT%H_%M_%S')}"

        print(f"Writing {filename} obj... ", end="", flush=True)
        try:
            o3d.io.write_triangle_mesh(os.path.join(
                output_folder, f"{filename}.obj"), mesh)
            print("Done")
        except:
            print(f"Could not write {filename}.obj")
        print(f"Writing {filename} ply... ", end="", flush=True)
        try:
            o3d.io.write_triangle_mesh(os.path.join(
                output_folder, f"{filename}.ply"), mesh)
            print("Done")
        except:
            print(f"Could not write {filename}.obj")

    def cloud_visualize(self):
        self.visualize(self.pcd)

    def cloud_output_png(self, filename="cloud"):
        self.output_png(self.pcd, filename)

    @staticmethod
    def output_png(mesh, filename):
        output_dir = "renders"
        os.makedirs(output_dir, exist_ok=True)
        now = datetime.datetime.now()
        filename = f"{filename}_{now.strftime('%Y_%m_%dT%H_%M_%S')}.png"

        vis = o3d.visualization.Visualizer()
        vis.create_window(visible=False)
        vis.add_geometry(mesh)
        vis.update_geometry(mesh)
        vis.poll_events()
        vis.update_renderer()
        vis.capture_screen_image(os.path.join(output_dir, filename))
        vis.destroy_window()

    def poisson(self, depth=8, scale=1.1, linear_fit=True):
        if self.poisson_mesh:
            return self.poisson_mesh
        if not self.pcd.has_normals():
            self.pcd.estimate_normals()
        print("Generating poisson mesh... ", end="", flush=True)
        self.poisson_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(
            self.pcd, depth=depth, scale=scale, linear_fit=linear_fit)[0]
        bbox = self.pcd.get_axis_aligned_bounding_box()
        self.poisson_mesh = self.poisson_mesh.crop(bbox)

        print("Done", flush=True)
        return self.poisson_mesh

    def poisson_write(self):
        self.write(self.poisson_mesh, "poisson")

    def poisson_visualize(self):
        self.visualize(self.poisson_mesh)

    def poisson_output_png(self, filename="poisson"):
        self.output_png(self.poisson_mesh, filename)

    def ballpoint(self):
        if self.ballpoint_mesh:
            return self.ballpoint_mesh
        if not self.pcd.has_normals():
            self.pcd.estimate_normals()
        print("Generating ballpoint mesh... ", end="", flush=True)
        distances = self.pcd.compute_nearest_neighbor_distance()
        avg_dist = np.mean(distances)
        radius = avg_dist
        radii = [0.5 * radius, radius, radius * 2]
        self.ballpoint_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(
            self.pcd, o3d.utility.DoubleVector(radii))
        print("Done", flush=True)
        return self.ballpoint_mesh

    def ballpoint_write(self):
        self.write(self.ballpoint_mesh, "ballpoint")

    def ballpoint_visualize(self):
        self.visualize(self.ballpoint_mesh)

    def ballpoint_output_png(self, filename="ballpoint"):
        self.output_png(self.ballpoint_mesh, filename)

    @staticmethod
    def clean_mesh(mesh):
        dec_mesh = mesh.simplify_quadric_decimation(100000)
        dec_mesh.remove_degenerate_triangles()
        dec_mesh.remove_duplicated_triangles()
        dec_mesh.remove_duplicated_vertices()
        dec_mesh.remove_non_manifold_edges()
        return dec_mesh


class EnterpriseLabel(Label):
    tooltip = ObjectProperty(None)

    def __init__(self, **kwargs):
        Window.bind(mouse_pos=self.on_mouse_pos)
        super(EnterpriseLabel, self).__init__(**kwargs)

    def on_mouse_pos(self, *args):
        if not self.get_root_window():
            return
        pos = args[1]
        # cancel scheduled event since I moved the cursor
        Clock.unschedule(self.display_tooltip)
        self.close_tooltip()  # close if it's opened
        if self.collide_point(*self.to_widget(*pos)):
            self.tooltip.pos = [pos[0] + 20, pos[1] - 30]
            Clock.schedule_once(self.display_tooltip, 1)

    def close_tooltip(self, *args):
        self.remove_widget(self.tooltip)

    def display_tooltip(self, dt):
        self.remove_widget(self.tooltip)
        self.add_widget(self.tooltip)


class Container(BoxLayout):

    connect_btn = ObjectProperty(None)
    display_btn = ObjectProperty(None)
    start_btn = ObjectProperty(None)
    stop_btn = ObjectProperty(None)
    one_btn = ObjectProperty(None)
    five_btn = ObjectProperty(None)
    ten_btn = ObjectProperty(None)
    debug_label = ObjectProperty(None)
    image = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(Container, self).__init__(*kwargs)
        '''super().__init__(**kwargs)
        self.isPortConnected = False
        self.portConnected = None'''
        self.connected = False
        self.ser = None
        self.do_read = False
        self.pcd = None
        self.image.anim_delay = 0.05
        self.image._coreimage.anim_reset(False)

    def threading_connection(self):
        threading.Thread(target=self.connectionProcess).start()

    def threading_start(self):
        threading.Thread(target=self.startProcess).start()

    def threading_stop(self):
        threading.Thread(target=self.stopProcess).start()

    def threading_display(self):
        threading.Thread(target=self.displayProcess).start()

    def threading_step(self, step):
        threading.Thread(target=self.step_setter, args=(step, )).start()

    def connectionProcess(self):
        if(self.connect_btn.text == 'Connect'):
            self.debug_label.text = 'Connecting...'
            ports_info = list_ports.comports()
            for element in ports_info:
                if ('Bluetooth' not in element.description):
                    try:
                        self.ser = serial.Serial(
                            port=element.name,
                            baudrate=115200,
                            bytesize=EIGHTBITS,
                            stopbits=STOPBITS_ONE,
                            xonxoff=True)
                        self.ser.write("v".encode())
                        time.sleep(1)
                        string = 0
                        string = self.ser.read_until("$".encode()).decode()

                        if(string == "Device succesfully connected$"):
                            self.isPortConnected = True
                            self.portConnected = element.name
                            time.sleep(1)
                            break
                        self.ser = None
                    except:
                        print(f"Error opening port {element.name}")

            if(self.ser and self.ser.is_open):
                self.connect_btn.text = 'Disconnect'
                self.debug_label.color = (61/255, 235/255, 52/255, 1)
                self.debug_label.text = 'Connected through port '+self.portConnected
                self.do_read = True

                # Enable all buttons after connecting
                self.connect_btn.disabled = False
                self.five_btn.disabled = False
                self.ten_btn.disabled = False
                self.start_btn.disabled = False
                self.stop_btn.disabled = True
                self.display_btn.disabled = True
            else:
                self.connect_btn.text = 'Connect'
                self.debug_label.color = (250/255, 250/255, 250/255, 1)
                self.debug_label.text = 'Waiting for connection'

        elif(self.connect_btn.text == 'Disconnect'):
            if self.ser != None:
                self.ser.close()
                self.ser = None
            self.isPortConnected = False
            self.portConnected = None
            self.connect_btn.text = 'Connect'
            self.debug_label.color = (250/255, 250/255, 250/255, 1)
            self.debug_label.text = 'Waiting for connection'

            # Disable all buttons after disconnecting
            self.connect_btn.disabled = False
            self.display_btn.disabled = True
            self.five_btn.disabled = True
            self.ten_btn.disabled = True
            self.start_btn.disabled = True
            self.stop_btn.disabled = True

    def displayProcess(self):
        print("Creating image...")
        if self.pcd == None:
            point_cloud = np.loadtxt(os.path.join("coordinates", self.filename), skiprows=1)
            self.pcd = PointCloud(point_cloud)
            self.pcd.ballpoint()
            matrix = self.pcd.ballpoint_mesh.get_rotation_matrix_from_xyz(
                (-np.pi / 3, 0, np.pi / 10))
            self.pcd.ballpoint_mesh.rotate(matrix)
        self.pcd.ballpoint_output_png()
        self.pcd.ballpoint_write()
        self.pcd.cloud_visualize()
        self.pcd.ballpoint_visualize()

    def reader(self):
        output_dir = "coordinates"
        os.makedirs(output_dir, exist_ok=True)
        now = datetime.datetime.now()
        self.filename = f"coordinates_{now.strftime('%Y_%m_%dT%H_%M_%S')}.xyz"
        with open(os.path.join(output_dir, self.filename), "w") as output_file:
            while self.do_read:
                # Wait until there is data waiting in the serial buffer
                if self.ser.in_waiting > 0:

                    # Read data out of the buffer until a carraige return / new line is found
                    # serialString = self.ser.read(self.ser.in_waiting)
                    serialString = self.ser.readline()
                    # serialString = self.ser.read_until("\n".encode())

                    # Print the contents of the serial data
                    try:
                        line = serialString.decode("utf-8").rstrip()
                        print(line)
                        if 'Done' in line:
                            self.image._coreimage.anim_reset(False)
                            self.debug_label.text = 'Done scanning around'
                            self.display_btn.disabled = False
                            self.do_read = False
                            self.connect_btn.disabled = False
                            self.five_btn.disabled = False
                            self.ten_btn.disabled = False
                            self.start_btn.disabled = False
                            self.stop_btn.disabled = True
                            break

                        output_file.write(line + '\n')
                        self.ser.flushInput()

                    except:
                        pass
                time.sleep(0.001)

    def startProcess(self):
        if(self.start_btn.text == 'Start'):
            self.image._coreimage.anim_reset(True)
            self.do_read = True
            self.connect_btn.disabled = True
            self.five_btn.disabled = True
            self.ten_btn.disabled = True
            self.start_btn.disabled = True
            self.stop_btn.disabled = False
            self.ser.write("b".encode())
            threading.Thread(target=self.reader, daemon=True).start()

    def stopProcess(self):
        if(self.stop_btn.text == 'Stop'):
            self.image._coreimage.anim_reset(False)
            self.do_read = False
            self.connect_btn.disabled = False
            self.five_btn.disabled = False
            self.ten_btn.disabled = False
            self.start_btn.disabled = False
            self.stop_btn.disabled = True
            self.ser.write('s'.encode())
            time.sleep(1)

    def step_setter(self, step):
        print(f"Setting step {step}")
        self.ser.write(step.encode())
        time.sleep(1)


class GuiApp(App):
    def build(self):
        return Container()


GuiApp().run()
