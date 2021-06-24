%GUI creates a .xyz file. Save and rename it as a .txt file in order to
%import it in MATLAB

%import .txt file with coordinates of point cloud
coord = importdata ('coordinates_plane_2.txt')

%% POINT CLOUD 
%create and display point cloud
ptCloud = pointCloud(coord);
figure
pcshow(ptCloud)
xlabel('X(m)')
ylabel('Y(m)')
zlabel('Z(m)')
title('Original Point Cloud')

maxDistance = 0.1;
[model,inlierIndices,outlierIndices,meanError] = pcfitplane(ptCloud,maxDistance) 

%% REGRESSION PLANE
x = coord(:,1)
y = coord(:,2)
z = coord(:,3)

figure(1)
stem3(x, y, z)
grid on
xv = linspace(min(x), max(x), 20);
yv = linspace(min(y), max(y), 20);
[X,Y] = meshgrid(xv, yv);
B = [x(:) y(:) ones(size(x(:)))] \ z(:);
Z = X*B(1) + Y*B(2) + B(3);
hold on
mesh(X, Y, Z)
hold off
view(150,24)
xlabel('X')
ylabel('Y')
zlabel('Z')
legend('Data', 'Linear Regression Plane', 'Location','southoutside')

%% TO REMOVE OUTLIERS BY HAND
% Specify you conditions
%Set conditions accordingly
TF1 = coord(:,2)>150 
TF2 = coord(:,2)<100
% remove
coord(TF1,:) = []
coord(TF2,:) = []

