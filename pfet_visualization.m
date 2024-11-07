clear; clc; close all;

EPSo = 8.854e-12;
ETAox = 3.9;
EPSox = EPSo * ETAox;

L = 180e-9;
MU = 35e-3;
Tox = 5e-9;
Vto = 0.4;
MUn = 35e-3;
MUp = 15e-3;
LAMBDA = 0.015;
Cox = EPSox / Tox;
BETA = 100;

w = 1e-6;
Vgs_sweep = Vto:0.05:1;
Vds = 0:0.0005:1;
dVgs = 0;
dVds = 0;

figure(); grid on; hold on;
xlabel("Vds"); ylabel("Id");
for Vgs = Vgs_sweep
    alpha = 1.0 ./ (1.0 + exp(-BETA .* (Vds - Vgs + Vto)));
    
    id_lin = MU .* Cox .* (w/L) .* ((Vgs - Vto) .* Vds - Vds.^2 ./ 2) .* (1 + LAMBDA .* Vds);
    [id_lin_max, id_lin_idx] = max(id_lin);

    id_sat = 0.5 .* MU .* Cox .* (w/L) .* (Vgs - Vto)^2 .* (1 + LAMBDA .* Vds);
    id = alpha .* id_sat + (1 - alpha) .* id_lin;

    % plot(Vds, id);
    plot(Vds, alpha);
end