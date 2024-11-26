clear; clc; close all;

% --- Support Functions ---------------------------------------------------
function fx = fx_smooth(BETA, GAMMA, fx1, fx2)
    gamma = GAMMA + 0.0;
    % gamma = GAMMA + 0.04;
    alpha = 1.0 ./ (1.0 + exp(-BETA .* gamma));
    fx = alpha .* fx1 + (1 - alpha) .* fx2;
end

% --- Id Functions --------------------------------------------------------
function Id_lin = getId_lin(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA)
    Id_lin = MU .* Cox .* (W/L) .* ((Vgs - Vt) .* Vds - Vds.^2 ./ 2) .* (1 + LAMBDA .* Vds);
end

function Id_sat = getId_sat(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA)
    Id_sat = 0.5 .* MU .* Cox .* (W/L) .* (Vgs - Vt)^2 .* (1 + LAMBDA .* Vds);
end

function [Id, Id_lin, Id_sat] = getId(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA)
    Id_lin = getId_lin(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    Id_sat = getId_sat(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    GAMMA = Vds - Vgs + Vt;
    Id = fx_smooth(BETA, GAMMA, Id_sat, Id_lin);
end

% --- Gm Analytic Model Functions -----------------------------------------
function [Gm] = getGm_analytic(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA)
    dVgs = 1e-3;
    Id0 = getId(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA);
    Id1 = getId(MU, Cox, W, L, Vgs + dVgs, Vds, Vt, LAMBDA, BETA);
    Gm = (Id1 - Id0) / dVgs;
end

% --- Gm Numeric Model Functions ------------------------------------------
function [Gm_lin, Id0_lin, Id1_lin] = getGm_lin(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA)
    dVgs = 1e-2;
    Id0_lin = getId_lin(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    Id1_lin = getId_lin(MU, Cox, W, L, Vgs + dVgs, Vds, Vt, LAMBDA);
    Gm_lin = (Id1_lin - Id0_lin) / dVgs;
end

function [Gm_sat, Id0_sat, Id1_sat] = getGm_sat(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA)
    dVgs = 1e-3;
    Id0_sat = getId_sat(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    Id1_sat = getId_sat(MU, Cox, W, L, Vgs + dVgs, Vds, Vt, LAMBDA);
    Gm_sat = (Id1_sat - Id0_sat) / dVgs;
end

function [Gm, Gm_lin, Gm_sat] = getGm_numeric(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA)
    Gm_lin = getGm_lin(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    Gm_sat = getGm_sat(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA);
    GAMMA = Vds - Vgs + Vt;
    Gm = fx_smooth(BETA, GAMMA, Gm_sat, Gm_lin);
end


EPSo = 8.854e-12;
ETAox = 3.9;
EPSox = EPSo * ETAox;

L = 180e-9;
Tox = 5e-9;
Vt = 0.4;
MUn = 35e-3;
MUp = 15e-3;
LAMBDA = 0.015;
Cox = EPSox / Tox;
BETA = 100;

W = 1e-6;
devType = "N";
if devType == "N"
    MU = MUn;
elseif devType == "P"
    MU = MUp;
end

% Vgs_sweep = Vt:0.05:1;
Vgs_sweep = Vt + 0.1;
Vds = 0:0.0005:1;
dVgs = 0;
dVds = 0;

figure();
grid on; hold on;
xlabel("Vds (V)");
hold("on");
for Vgs = Vgs_sweep
    [Id, Id_lin, Id_sat] = getId(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA);
    [Gm_analytic] = getGm_analytic(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA);
    [Gm_numeric, Gm_numeric_lin, Gm_numeric_sat] = getGm_numeric(MU, Cox, W, L, Vgs, Vds, Vt, LAMBDA, BETA);
    tx = Vgs - Vt;
    xline(tx);
    % plot(Vds, Id, Vds, Gm_analytic, Vds, Gm_numeric, Vds, Gm_numeric_lin, Vds, Gm_numeric_sat);
    % legend("Id", "Gm [analytic]", "Gm [numeric]", "Gm [num-lin]", "Gm [num-sat]");
    % plot(Vds, Id, Vds, Gm_analytic, Vds, Gm_numeric);
    % legend("Id", "Gm [analytic]", "Gm [numeric]");
    plot(Vds, Gm_analytic, Vds, Gm_numeric, Vds, Gm_numeric_lin, Vds, Gm_numeric_sat);
    legend("Gm [analytic]", "Gm [numeric]", "Gm [num-lin]", "Gm [num-sat]");
    ylim([0, max([max(Gm_analytic), max(Gm_numeric)])]);
    % plot(Vds, Gm_analytic, Vds, Gm_numeric);
    % legend("Gm [analytic]", "Gm [numeric]");
end

test_cases = [
    Vt - 0.1, Vt + 0.01, Vt + 0.2, Vt + 0.5, Vt + 0.7;
    0.5, 0.0, 0.19, 0.5, Vt + 0.7;
    "Cutoff", "Lin_BarelyOn", "Lin_AlmostSat", "Sat_Barely", "Sat_Deep"
];

for tc = test_cases
    Gm_analytic = getGm_analytic(MU, Cox, W, L, str2double(tc(1)), str2double(tc(2)), Vt, LAMBDA, BETA);
    [Gm_numeric, Gm_numeric_lin, Gm_numeric_sat] = getGm_numeric(MU, Cox, W, L, str2double(tc(1)), str2double(tc(2)), Vt, LAMBDA, BETA);
    fprintf("mode: \033[92m%s\033[0m\n", tc(3));
    fprintf("\b\033[91mvgs\033[0m: %.12f | \033[31mvds\033[0m: %.12f\n", str2double(tc(1)), str2double(tc(2)));
    fprintf("\b\033[91mgma\033[0m: %.12f | \033[31mgmn\033[0m: %.12f\n", Gm_analytic, Gm_numeric);
    fprintf("\b\033[91mgml\033[0m: %.12f | \033[31mgms\033[0m: %.12f\n\n", Gm_numeric_lin, Gm_numeric_sat);
end
