import numpy as np
import open3d as o3d
import os
import argparse

# TODO: Ugly global variables
ARGS = None
PCD = None
OUTPUT_FOLDER="files/out/"

def argparse_init() -> None:
    global ARGS

    parser = argparse.ArgumentParser(
        description="Produce a mesh obj/ply file from an xyz file")
    parser.add_argument(
        "-v", "--version", action="version",
        version=f"{parser.prog} version 0.5.0"
    )

    parser.add_argument("FILE", help="The xyz file to read")

    parser.add_argument("-c", "--show-cloud", help="Only output the xyz point formation", action="store_true")
    parser.add_argument("-p", "--poisson", help="Generate the mesh using Poisson surface reconstruction", action="store_true")
    parser.add_argument("-b", "--ballpoint", help="Generate the mesh using Ball pivoting", action="store_true")
    parser.add_argument("-r", "--render", help="Output every generated mesh", action="store_true")
    # parser.add_argument("-a", "--alpha" help="Generate the mesh using Alpha shapes", action="store_true")

    ARGS = parser.parse_args()


def open_file() -> bool:
    # TODO: Read normals from file
    # TODO: Read colors from file
    global PCD
    try:
        print("Loading point array... ", end="", flush=True)
        point_cloud = np.loadtxt(ARGS.FILE, skiprows=1)
        print("Done")

        print("Reading points and creating point cloud... ", end="", flush=True)
        PCD = o3d.geometry.PointCloud()
        PCD.points = o3d.utility.Vector3dVector(point_cloud[:, :3])
        print("Done")
        return True
    except Exception as e:
        print(e)
        return False


def poisson():
    global PCD
    if not PCD.has_normals():
        PCD.estimate_normals()
    print("Generating poisson mesh... ", end="", flush=True)
    mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(PCD, depth=8, scale=1.1, linear_fit=True)[0]
    bbox = PCD.get_axis_aligned_bounding_box()
    mesh = mesh.crop(bbox)
    print("Done", flush=True)
    return mesh


def ballpoint():
    global PCD
    if not PCD.has_normals():
        PCD.estimate_normals()
    print("Generating ballpoint mesh... ", end="", flush=True)
    distances = PCD.compute_nearest_neighbor_distance()
    avg_dist = np.mean(distances)
    radius = 3 * avg_dist
    mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(PCD, o3d.utility.DoubleVector([radius, radius * 2]))
    print("Done", flush=True)
    return mesh


def clean_mesh(mesh):
    dec_mesh = mesh.simplify_quadric_decimation(100000)
    dec_mesh.remove_degenerate_triangles()
    dec_mesh.remove_duplicated_triangles()
    dec_mesh.remove_duplicated_vertices()
    dec_mesh.remove_non_manifold_edges()
    return dec_mesh


def write(mesh, filename = "mesh"):
    print("Writing obj... ", end="", flush=True)
    o3d.io.write_triangle_mesh(os.path.join(OUTPUT_FOLDER, f"{filename}.obj"), mesh)
    print("Done")
    print("Writing ply... ", end="", flush=True)
    o3d.io.write_triangle_mesh(os.path.join(OUTPUT_FOLDER, f"{filename}.ply"), mesh)
    print("Done")


def visualize(mesh):
    vis = o3d.visualization.Visualizer()
    vis.create_window()
    vis.add_geometry(mesh)
    vis.run()
    vis.destroy_window()


if __name__ == '__main__':
    argparse_init()
    if not open_file():
        exit(0)
    if ARGS.show_cloud:
        o3d.visualization.draw_geometries([PCD])
    if ARGS.poisson:
        mesh = poisson()
        write(mesh, "poisson")
        if(ARGS.render):
            visualize(mesh)
    if ARGS.ballpoint:
        mesh = ballpoint()
        write(mesh, "ballpoint")
        if(ARGS.render):
            visualize(mesh)
