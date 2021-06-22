from kivy.app import App
from kivy.lang import Builder
from kivy.uix.widget import Widget
from kivy.core.window import Window
from kivy.uix.actionbar import ActionButton
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from kivy.properties import StringProperty
from serial.serialutil import SerialException
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

class PointCloud:
    # TODO: alpha non visualizza
    def __init__(self, coordinates):
        self.pcd = o3d.geometry.PointCloud()
        self.pcd.points = o3d.utility.Vector3dVector(coordinates[:, :3])
        self.poisson_mesh = None
        self.ballpoint_mesh = None
        self.alpha_mesh = None
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
        vis.run()
        vis.destroy_window()

    @staticmethod
    def write(mesh, filename="mesh", output_folder=""):
        print(f"Writing {filename} obj... ", end="", flush=True)
        o3d.io.write_triangle_mesh(os.path.join(
            output_folder, f"{filename}.obj"), mesh)
        print("Done")
        print(f"Writing {filename} ply... ", end="", flush=True)
        o3d.io.write_triangle_mesh(os.path.join(
            output_folder, f"{filename}.ply"), mesh)
        print("Done")

    def cloud_visualize(self):
        self.visualize(self.pcd)

    def cloud_output_png(self, filename="cloud.png"):
        self.output_png(self.pcd, filename)

    @staticmethod
    def output_png(mesh, filename):
        #depth = vis.capture_depth_float_buffer(False)
        #image = vis.capture_screen_float_buffer(False)
        #plt.imsave("../../test_data/depth/{:05d}.png".format(glb.index), np.asarray(depth), dpi = 1)
        vis = o3d.visualization.Visualizer()
        vis.create_window(visible=False)
        vis.add_geometry(mesh)
        vis.update_geometry(mesh)
        vis.poll_events()
        vis.update_renderer()
        vis.capture_screen_image(filename)
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

        #R = self.poisson_mesh.get_rotation_matrix_from_xyz((np.pi, 0, 0))
        #self.poisson_mesh.rotate(R, center=(0, 0, 0))

        print("Done", flush=True)
        return self.poisson_mesh

    def poisson_write(self, output_folder=""):
        self.write(self.poisson_mesh, "poisson", output_folder)

    def poisson_visualize(self):
        self.visualize(self.poisson_mesh)

    def poisson_output_png(self, filename="poisson.png"):
        self.output_png(self.poisson_mesh, filename)

    def ballpoint(self):
        if self.ballpoint_mesh:
            return self.ballpoint_mesh
        if not self.pcd.has_normals():
            self.pcd.estimate_normals()
        print("Generating ballpoint mesh... ", end="", flush=True)
        distances = self.pcd.compute_nearest_neighbor_distance()
        avg_dist = np.mean(distances)
        radius = 3 * avg_dist
        self.ballpoint_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(
            self.pcd, o3d.utility.DoubleVector([radius, radius * 2]))
        print("Done", flush=True)
        return self.ballpoint_mesh

    def ballpoint_write(self, output_folder=""):
        self.write(self.ballpoint_mesh, "ballpoint", output_folder)

    def ballpoint_visualize(self):
        self.visualize(self.ballpoint_mesh)

    def ballpoint_output_png(self, filename="ballpoint.png"):
        self.output_png(self.ballpoint_mesh, filename)

    def alpha(self, alpha=0.03):
        if self.alpha_mesh:
            return self.alpha_mesh
        if not (self.tetra_mesh and self.pt_map):
            self.tetra_mesh, self.pt_map = o3d.geometry.TetraMesh.create_from_point_cloud(
                self.pcd)
        print("Generating alpha shapes mesh... ", end="", flush=True)
        self.alpha_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_alpha_shape(
            self.pcd, alpha, self.tetra_mesh, self.pt_map)
        self.alpha_mesh.compute_vertex_normals()
        print("Done", flush=True)
        return self.alpha_mesh

    def alpha_write(self, output_folder=""):
        self.write(self.alpha_mesh, "alpha", output_folder)

    def alpha_visualize(self):
        self.visualize(self.alpha_mesh)

    def alpha_output_png(self, filename="alpha.png"):
        self.output_png(self.alpha_mesh, filename)

    @staticmethod
    def clean_mesh(mesh):
        dec_mesh = mesh.simplify_quadric_decimation(100000)
        dec_mesh.remove_degenerate_triangles()
        dec_mesh.remove_duplicated_triangles()
        dec_mesh.remove_duplicated_vertices()
        dec_mesh.remove_non_manifold_edges()
        return dec_mesh


class Container(BoxLayout):

    connect_btn = ObjectProperty(None)
    #com_ports_spinner = ObjectProperty(None)
    display_btn=ObjectProperty(None)
    start_btn = ObjectProperty(None)
    stop_btn = ObjectProperty(None)
    one_btn = ObjectProperty(None)
    five_btn = ObjectProperty(None)
    ten_btn = ObjectProperty(None)
    image = ObjectProperty(None)
    debug_label = ObjectProperty(None)
    img_src = StringProperty('images/cloud.png')

    # NON SONO SICURO QUESTA FUNZIONE SERVA
    def __init__(self, **kwargs):
        '''super().__init__(**kwargs)
        self.isPortConnected = False
        self.portConnected = None'''
        self.connected = False
        self.ser = None
        self.do_read = False
        super(Container, self).__init__(*kwargs)

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

    def threading_ten(self):
        threading.Thread(target=self.tenProcess).start()

    def connectionProcess(self):
        if(self.connect_btn.text == 'Connect'):
            self.debug_label.text = 'Connecting...'
            ports_info = list_ports.comports()
            for element in ports_info:
                if ('Bluetooth' not in element.description):
                    try:
                        self.ser = serial.Serial(
                            port=element.name, baudrate=115200, timeout=0.5)
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
                self.ser.timeout = 0
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
        point_cloud = np.loadtxt("coordinates.xyz", skiprows=1)
        pcd = PointCloud(point_cloud)
        pcd.cloud_visualize()
        pcd.cloud_output_png("images/cloud.png")
        image.source=img_src

    
    def reader(self):
        with open("coordinates.xyz", "w") as output_file:
            while self.do_read:
                # Wait until there is data waiting in the serial buffer
                if self.ser.in_waiting > 0:

                    # Read data out of the buffer until a carraige return / new line is found
                    #serialString = self.ser.read(self.ser.in_waiting)
                    serialString = self.ser.readline()
                    #serialString = self.ser.read_until("\n".encode())
                    
                    # Print the contents of the serial data
                    try:
                        line = serialString.decode("utf-8").rstrip()
                        print(line)
                        if 'Done' in line:
                            self.debug_label.text = 'Done scanning around'
                            self.display_btn.disabled=False
                            self.do_read = False
                            #self.display_btn.disabled=False
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
            self.do_read = True
            self.connect_btn.disabled = True
            self.five_btn.disabled = True
            self.ten_btn.disabled = True
            self.start_btn.disabled = True
            self.stop_btn.disabled = False
            self.ser.write("b".encode())
            threading.Thread(target=self.reader).start()

    def stopProcess(self):
        if(self.stop_btn.text == 'Stop'):
            self.do_read = False
            #self.display_btn.disabled=False
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



