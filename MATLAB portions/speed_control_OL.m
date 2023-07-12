K=0.205724771;
Ra=7.326161;
J=2.384*(10^(-4));
bv=1.942041667;
bm=0.000413802;
M=0.367;
num_m = [K 0];
den_m = [(J*Ra) ((bm*Ra)+(K^2)) 0];
M_s = tf(num_m, den_m)
den_v= [M bv];
V_s = tf(2, den_v)
OL_speed_sys = series(M_s, V_s)
CL_speed_sys = feedback(OL_speed_sys,1,-1)
hold on;
rlocus(OL_speed_sys)
figure
bode(OL_speed_sys)
figure
Tfinal=10;
step(CL_speed_sys)
%stepinfo(CL_speed_sys)
ramp_sys=series(tf(1,[1 0]),CL_speed_sys)
figure
step(ramp_sys)