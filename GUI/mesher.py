import numpy as np
import open3d as o3d
import os
import argparse


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


def argparse_init() -> argparse.ArgumentParser:
    # TODO: render option without b or p should trigger a warning

    parser = argparse.ArgumentParser(
        description="Produce a mesh obj/ply file from an xyz file")
    parser.add_argument(
        "-v", "--version", action="version",
        version=f"{parser.prog} version 0.5.0"
    )

    parser.add_argument("FILE", help="The xyz file to read")

    parser.add_argument(
        "-p", "--poisson", help="Generate the mesh using Poisson surface reconstruction", action="store_true")
    parser.add_argument(
        "-b", "--ballpoint", help="Generate the mesh using Ball pivoting", action="store_true")
    parser.add_argument(
        "-a", "--alpha", help="Generate the mesh using alpha shapes", action="store_true")
    parser.add_argument(
        "-r", "--render", help="Output every generated mesh", action="store_true")
    parser.add_argument(
        "-s", "--save", help="Save every generated mesh in PLY and OBJ formats", action="store_true")
    parser.add_argument(
        "-o", "--output", help="Save a PNG for every generated mesh", action="store_true")

    return parser.parse_args()


def open_cloud_file(filename: str) -> PointCloud:
    # TODO: Read normals from file
    # TODO: Read colors from file
    # TODO: Integrate this into PointCloud class
    try:
        print("Loading point array... ", end="", flush=True)
        point_cloud = np.loadtxt(filename, skiprows=1)
        print("Done")

        print("Reading points and creating point cloud... ", end="", flush=True)
        pcd = PointCloud(point_cloud)
        print("Done")
        return pcd
    except Exception as e:
        print(e)
        return None


if __name__ == '__main__':
    args = argparse_init()
    pcd = open_cloud_file(args.FILE)
    if not pcd:
        exit(0)
    if args.render:
        pcd.cloud_visualize()
    if args.output:
        pcd.cloud_output_png("files/out/cloud.png")
    if args.poisson:
        pcd.poisson()
        if args.save:
            pcd.poisson_write("files/out")
        if args.render:
            pcd.poisson_visualize()
        if args.output:
            pcd.poisson_output_png("files/out/poisson.png")
    if args.ballpoint:
        pcd.ballpoint()
        if args.save:
            pcd.ballpoint_write("files/out")
        if args.render:
            pcd.ballpoint_visualize()
        if args.output:
            pcd.ballpoint_output_png("files/out/ballpoint.png")
    if args.alpha:
        pcd.alpha()
        if args.save:
            pcd.alpha_write("files/out")
        if args.render:
            pcd.alpha_visualize()
        if args.output:
            pcd.alpha_output_png("files/out/alpha.png")
