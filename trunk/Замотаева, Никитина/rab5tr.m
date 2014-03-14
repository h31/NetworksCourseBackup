t=-0.04:1/1000:0.04;
y4=-5*rectpuls(t,0.02);
y5=-10*rectpuls(t,0.03);
y6= conv(y4,y5);
plot(y6);
figure;
spectrum = abs(fft(y6,1024))/1024;
plot(spectrum);