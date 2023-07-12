K=0.205724771;
Ra=7.326161;
J=2.384*(10^(-4));
bv=1.942041667;
bm=0.000413802;
M=0.367;
r=0.0335;
num_m = [J*K bm*K];
den_m = [(J*Ra) ((bm*Ra)+(K^2))];
M_s = tf(num_m, den_m)
den_v= [M*r bv*r];
V_s = tf(2, den_v)
PID_control=pid(0.9981,221.8,0.0008872);
OL_speed_sys = series(series(M_s, V_s),PID_control)
CL_speed_sys = feedback(OL_speed_sys,1,-1)
ramp_sys=series(tf(1,[1 0]),CL_speed_sys)
OL_speed_sys_d = c2d(OL_speed_sys,0.1)
CL_speed_sys_d = c2d(CL_speed_sys,0.1)
ramp_sys_d = c2d(ramp_sys,0.1)
PID_control_d = c2d(PID_control,0.1,'tustin')
rlocus(OL_speed_sys_d)
figure
bode(OL_speed_sys_d)
figure
step(CL_speed_sys_d)
stepinfo(CL_speed_sys_d)
figure
step(ramp_sys_d)
figure
pzmap(CL_speed_sys_d)
