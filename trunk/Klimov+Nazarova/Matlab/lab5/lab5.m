t = 0:00.1:4*pi;
N = 100;
y = 0;

% Полигармонический сигнал
y = sin(pi*t)+sin(2*pi*t)+sin(pi*0.3*t);
plot(t,y,'LineWidth',2)
grid

figure
spectrum = fft(y,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:255)/512;
plot(f, norm_spectrum(1:256),'LineWidth',2)
axis([0 max(f) 0 10])
grid

% Прямоугольный сигнал
figure
y1 = square(t,50);
plot(t(1:100),y1(1:100),'LineWidth',2);
ylim([-2,2]);
grid

figure
spectrum = fft(y1,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f1=100*(0:255)/512;
plot(f1, norm_spectrum(1:256),'LineWidth',2)
axis([0 max(f1) 0 10])
grid

% Треугольный сигнал
figure
y2 = conv(square(t,20),square(t,20));
plot(t(1:100),y2(1:100),'LineWidth',2);
grid

figure
spectrum = fft(y2,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f2=100*(0:255)/512;
plot(f2, norm_spectrum(1:256)/1000,'LineWidth',2)
axis([0 max(f2) 0 50])
grid

% x = 0:0.01:4*pi; 
% f=100*(0:255)/512;  
% figure 
% y1 = sin(2*pi*x)+sin(2*pi*3*x); 
% plot(x(1:200),y1(1:200)) 
% grid 
% figure 
% s1 = fft(y1,512); 
% ss1 = s1.*conj(s1)/512; 
% plot(f,ss1(1:256)) 
% grid  
% figure 
% y2 = sin(2*pi*x)+cos(2*pi*x);
% plot(x(1:200),y2(1:200))   
% grid  
% figure 
% s2 = fft(y2,512); 
% ss2 = s2.*conj(s2)/512; 
% plot(f,ss2(1:256)) 
%  
% t=-0.04:1/1000:0.04; 
% y4=-5*rectpuls(t,0.04); 
% plot(t,y4); 
% figure; 
% spectrum = abs(fft(y4,1024))/1024; 
% plot(spectrum); 
% 
% t=-0.04:1/1000:0.04; 
% y4=-5*rectpuls(t,0.02); 
% y5=-10*rectpuls(t,0.03); 
% y6= conv(y4,y5); 
% plot(y6); 
% figure; 
% spectrum = abs(fft(y6,1024))/1024;
% plot(spectrum); 