%GUI creates a .xyz file. Save and rename it as a .txt file in order to
%import it in MATLAB

%import .txt file with coordinates of point cloud
coord = importdata ('coordinates_plane_2.txt')

%% POINT CLOUD 
%DISCLAIMER: point cloud functions need to have computer vision toolbox
%installed in your environment. Further ahead you can find the code to
%compute a regression plane without the need for computer vision toolbox
%create and display point cloud
ptCloud = pointCloud(coord);
figure
pcshow(ptCloud)
xlabel('X(m)')
ylabel('Y(m)')
zlabel('Z(m)')
title('Original Point Cloud')