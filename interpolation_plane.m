coord = importdata ('coordinates_plane.txt')

% Specify you conditions
TF1 = coord(:,2)>200 
TF2 = coord(:,2)<100
% remove
coord(TF1,:) = []
coord(TF2,:) = []

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
