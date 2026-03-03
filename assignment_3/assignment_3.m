% 1. Finite-differences

% Creates the graph for part (a)
h_values1 = [1/6, 1/3, 1/2, 2/3, 1];
t_max = 1;
colors = lines(length(h_values1));

figure; hold on; grid on;

for i = 1:length(h_values1)
    h = h_values1(i);
    
    t = 0:h:t_max;
    u = zeros(size(t));
    u(1) = 1;
    
    for n = 1:(length(t)-1)
        u(n+1) = (1 - 3*h)*u(n) + 2*h;
    end
    
    plot(t, u, 'o-', 'Color', colors(i,:), 'LineWidth', 1.5, ...
         'MarkerFaceColor', colors(i,:), 'DisplayName', sprintf('h = %.3f', h));
end

xlabel('t');
ylabel('u_f(nh)');
title('Forward Euler Approximation of du/dt = -3u(t) + 2');
legend('Location', 'northeast');
axis([0 1 -0.2 1.1]);
hold off;

%% 

% 2. Iterative solution of linear systems

% Jacobi
n = 10;
approximations = zeros(2, n);

% Compute approximations for x and y for part (b)
for i = 2:n
    x_prev = approximations(1, i - 1);
    y_prev = approximations(2, i - 1);

    approximations(1, i) = (6 - 2 * y_prev) / 4;
    approximations(2, i) = (4 + x_prev) / 5;

end

disp('Jacobi:')
disp(approximations)

% 3D plot of (x, y, i) for part (b)
figure;
plot3(approximations(1, :), approximations(2, :), 1:n, 'o-');
xlabel('x-value');
ylabel('y-value');
zlabel('iteration number');
title('3D Plot of Approximations');
grid on;

% Gauss-Seidel
n = 10;
approximations_c = zeros(2, n);

% Compute approximations for x and y for part (c)
for i = 2:n
    x_prev = approximations_c(1, i - 1);
    y_prev = approximations_c(2, i - 1);

    x = (6 - 2 * y_prev) / 4;
    y = (4 + x) / 5;

    approximations_c(1, i) = x;
    approximations_c(2, i) = y;

end

disp('Gauss-Seidel:')
disp(approximations_c)

% 3D plot of (x, y, i) for part (c)
figure;
plot3(approximations_c(1, :), approximations_c(2, :), 1:n, 'o-');
xlabel('x-value');
ylabel('y-value');
zlabel('iteration number');
title('3D Plot of Approximations (Gauss-Seidel)');
grid on;

%%

% 3. ODE's

% Forward-Euler Discretization
h_values3 = [0.01, 0.1, 1.0, 2.0];
x_end = 2 * pi;

for i = 1:length(h_values3)
    h = h_values3(i);
    x = 0:h:x_end;
    n_points = length(x);
    y = zeros(1, n_points);
    
    % Initial conditions discretized in part (b)
    y(1) = 0;
    y(2) = h;
    
    for n = 1:(n_points - 2)
        y(n+2) = 2*y(n+1) - (1 + h^2)*y(n);
    end
    
    % Create separate graphs for each h
    figure(i);
    plot(x, y, '-o', 'DisplayName', 'Solution using forward difference');
    hold on;
    fplot(@(t) sin(t), [0 x_end], 'r--', 'LineWidth', 1.5, 'DisplayName', 'Exact (y = sin(x))');
    title(['Forward-Euler Solution for h = ', num2str(h)]);
    xlabel('x'); ylabel('y');
    xlim([0 2*pi]);
    legend; grid on;
end

% Backward-Euler discretization
x_end = 2 * pi;

for i = 1:length(h_values3)
    h = h_values3(i);
    x = 0:h:x_end;
    n_points = length(x);
    y_b = zeros(1, n_points);
    
    % Initial conditions
    y_b(1) = 0;
    y_prev = -h;
    
    y_b(2) = (2*y_b(1) - y_prev) / (1 + h^2);

    for n = 3:n_points
        y_b(n) = (2*y_b(n-1) - y_b(n-2)) / (1 + h^2);
    end
    
    % Create separate graphs for each h
    figure(i + 10); 
    plot(x, y_b, '-o', 'DisplayName', 'Solution using backward difference');
    hold on;
    fplot(@(t) sin(t), [0 x_end], 'r--', 'LineWidth', 1.5, 'DisplayName', 'Exact (y = sin(x))');
    title(['Backward-Euler Solution for h = ', num2str(h)]);
    xlabel('x'); ylabel('y');
    xlim([0 2*pi]);
    legend; grid on;
end

%%

% 4. PDE's

n = 5; % Number of interior points per row/column
N = n^2; % Total interior points
A = zeros(N, N);
b = zeros(N, 1);

% Boundary temperatures
T_top = 100; T_bottom = 300; T_left = 50; T_right = 200;

for j = 1:n
    for i = 1:n
        row = (j-1)*n + i;
        A(row, row) = -4;
        
        % Left neighbor (i-1)
        if i > 1
            A(row, row-1) = 1;
        else 
            b(row) = b(row) - T_left; 
        end

        % Right neighbor (i+1)
        if i < n
            A(row, row+1) = 1;
        else
            b(row) = b(row) - T_right;
        end

        % Bottom neighbor (j-1)
        if j > 1
            A(row, row-n) = 1;
        else
            b(row) = b(row) - T_bottom;
        end

        % Top neighbor (j+1)
        if j < n
            A(row, row+n) = 1;
        else
            b(row) = b(row) - T_top;
        end
    end
end

% Solve the linear system
u_interior = A \ b;
results = reshape(u_interior, n, n)';
results = flipud(results);
disp(results);

%%

% 5. PDE's

% Parameters
dx = 0.01;
dy = 0.01;
dt = 0.0025;
steps = 500;

rx2 = (dt / dx)^2;
ry2 = (dt / dy)^2;

% Grid setup
x = 0:dx:1;
y = 0:dy:1;
[X, Y] = meshgrid(x, y);

nx = length(x);
ny = length(y);

% Initial conditions
u0 = 4 .* (X.^2) .* Y .* (1 - X) .* (1 - Y);
u1 = zeros(ny, nx);

% Track the center displacement 
mid_idx = 51;
center_displacement = zeros(1, steps + 1);
center_displacement(1) = u0(mid_idx, mid_idx);

% First time step
for i = 2:ny-1
    for j = 2:nx-1
        u_x = rx2 * (u0(i, j+1) - 2*u0(i, j) + u0(i, j-1));
        u_y = ry2 * (u0(i+1, j) - 2*u0(i, j) + u0(i-1, j)); 
        u1(i, j) = u0(i, j) + 0.5 * (u_x + u_y);
    end
end

center_displacement(2) = u1(mid_idx, mid_idx);

% Initialize previous and current states
u_prev = u0;
u_curr = u1;
u_next = zeros(ny, nx);
u_200 = zeros(ny, nx);

% Remaining values for u
for n = 2:steps
    for i = 2:ny-1
        for j = 2:nx-1
            u_x = rx2 * (u_curr(i, j+1) - 2*u_curr(i, j) + u_curr(i, j-1));
            u_y = ry2 * (u_curr(i+1, j) - 2*u_curr(i, j) + u_curr(i-1, j));
            
            u_next(i, j) = 2*u_curr(i, j) - u_prev(i, j) + u_x + u_y;
        end
    end
    
    % Save the state at t=200 for plotting
    if n == 200
        u_200 = u_next;
    end
    
    center_displacement(n + 1) = u_next(mid_idx, mid_idx);
    
    % Shift arrays forward for the next time step
    u_prev = u_curr;
    u_curr = u_next;
end

% Plot for t = 200 (5c)
figure('Name', 'Displacement at t = 200');
surf(X, Y, u_200);
shading interp;
colormap jet;
title('t = 200');
xlabel('x');
ylabel('y');
zlabel('u(x,y,200)');
zlim([-0.15 0.15]);
view([-37.5, 30]);

% Plot for center displacement over time (5d)
figure('Name', 'Center Displacement Over Time');
plot(0:steps, center_displacement, 'b-', 'LineWidth', 1.5);
title('Displacement of Point (0.5, 0.5) Over Time');
xlabel('Time step (t)');
ylabel('u(0.5, 0.5, t)');
grid on;