%GUI creates a .xyz file. Save and rename it as a .txt file in order to
%import it in MATLAB

%import .txt file with coordinates of point cloud
coord = importdata (strcat(pwd,'\coordinates\coordinates_plane_2.txt'))

%% POINT CLOUD 
%DISCLAIMER: point cloud functions need to have computer vision toolbox
%installed in your environment. Further ahead you can find the code to
%compute a regression plane without the need for computer vision toolbox
%create and display point cloud
ptCloud = pointCloud(coord);
figure
pcshow(ptCloud)
xlabel('X(mm)')
ylabel('Y(mm)')
zlabel('Z(mm)')
title('Original Point Cloud')

%native MATLAB function to fit a regression plane and compute MSE (mean
%square error)
maxDistance = 0.1;
[model,inlierIndices,outlierIndices,meanError] = pcfitplane(ptCloud,maxDistance) 


%%
%{
az = 0;
el = 90;
view([az,el])


degStep = 5;
detlaT = 0.1;
fCount = 71;
f = getframe;
[im,map] = rgb2ind(f.cdata,256);
k = 1;

for i = 0:-degStep:-45
  az = i;
  ([az,el])
  f = getframe(gcf);
  im(:,:,1,k) = rgb2ind(f.cdata,map);
  k = k + 1;
end
% tilt down
for i = 90:-degStep:15
  el = i;
  view([az,el])
  f = getframe;
  im(:,:,1,k) = rgb2ind(f.cdata,256);
  
  k = k + 1;
end
% spin left
for i = az:-degStep:-90
  az = i;
  view([az,el])
  f = getframe;
  im(:,:,1,k) = rgb2ind(f.cdata,256);
  k = k + 1;
end
% spin right
for i = az:degStep:0
  az = i;
  view([az,el])
  f = getframe;
  im(:,:,1,k) = rgb2ind(f.cdata,256);
  k = k + 1;
end
% tilt up to original
for i = el:degStep:90
  el = i;
  view([az,el])
  f = getframe;
  im(:,:,1,k) = rgb2ind(f.cdata,256);
  k = k + 1;
end
imwrite(im,map,'Animation.gif','DelayTime',detlaT,'LoopCount',inf)


%}

%% REGRESSION PLANE
%another method to compute regression plane that does NOT need Computer
%Vision Toolbox

%save x y z vectors
x = coord(:,1)
y = coord(:,2)
z = coord(:,3)

%plot x y z points
figure(1)
%stem3(x, y, z)
scatter3(x,y,z)
grid on
xv = linspace(min(x), max(x), 20);
yv = linspace(min(y), max(y), 20);
[X,Y] = meshgrid(xv, yv);
%regression plane equation
B = [x(:) y(:) ones(size(x(:)))] \ z(:);
Z = X*B(1) + Y*B(2) + B(3);
hold on
mesh(X, Y, Z)
hold off
view(200,10)
xlabel('X')
ylabel('Y')
zlabel('Z')
legend('Data', 'Linear Regression Plane', 'Location','southoutside')




%% TO REMOVE OUTLIERS BY HAND
% Specify you conditions
%Set conditions accordingly
TF1 = coord(:,2)>150 
TF2 = coord(:,2)<150
% remove
coord(TF1,:) = []
coord(TF2,:) = []

