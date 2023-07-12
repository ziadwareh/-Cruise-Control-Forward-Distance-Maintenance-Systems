K=0.205724771;
Ra=7.326161;
J=2.384*(10^(-4));
bv=2.344722222;
bm=0.000413802;
M=0.367;
r=0.0335;
num_m = [J*K bm*K];
den_m = [(J*Ra) ((bm*Ra)+(K^2))];
M_s = tf(num_m, den_m)
den_v= [M*r bv*r];
V_s = tf(2, den_v)
%PID_control=pid(0.9981,221.8,0.0008872);
control=tf([0.0008872 0.9981 221.8],[1 0])
PI_control=pid(0.15729,1.1235,0);
OL_speed_sys = series(series(M_s, V_s),series(control,PI_control))
CL_speed_sys = feedback(OL_speed_sys,1,-1)
step(CL_speed_sys)
ramp_sys=series(tf(1,[1 0]),CL_speed_sys)
figure
step(ramp_sys)
%pzmap(CL_speed_sys)
%pole(CL_speed_sys)
%zero(CL_speed_sys)
%sisotool(OL_speed_sys)