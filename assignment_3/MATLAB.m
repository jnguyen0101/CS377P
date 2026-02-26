% 1. Finite-differences

% Creates the graph for part (a)
h_values = [1/6, 1/3, 1/2, 2/3, 1];
t_max = 1;
colors = lines(length(h_values));

figure; hold on; grid on;

for i = 1:length(h_values)
    h = h_values(i);
    
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