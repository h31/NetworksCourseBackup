t=-0.04:1/1000:0.04;
y4=-5*rectpuls(t,0.04);
plot(t,y4);
figure;
spectrum = abs(fft(y4,1024))/1024;
plot(spectrum);