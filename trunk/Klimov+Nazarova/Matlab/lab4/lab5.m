% f = 10;
% x = 0:1/(2*pi):8*pi;%4 periods
% y = cos(2*pi*f*x);
% plot(x,y)
% grid
% figure
% spectrum = fft(y,512);
% spectrum = abs(fft(y,512));
% norm_spectrum = spectrum.*conj(spectrum)/512;%спектр умн на его комплексное сопряженное - нормир спектр
% f=100*(0:511)/512;
% plot(f, norm_spectrum(1:512))
% axis([0 max(f) 0 10])
% grid
t = 0:0.1:4*pi;
N=20;
for i=1:length(t)
    y(i) = cos(2*pi*i*t(i));
end;
plot(t,y)
grid

spectrum = fft(y,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:511)/512;
figure
plot(f, norm_spectrum(1:512))
axis([0 max(f) 0 10])
grid

% y_n=[0 1 1 0 0 1 1 0 0];
% x_n=[1 1 2 2 3 3 4 4 5];

x_n=0:0.01:4*pi;
f0=5;
for i=1:length(x_n)
    if mod(i,60) <3
        0 
        y_n(i)=0;
    else
        y_n(i)= 1;
    end;
end;
figure
plot(x_n, y_n)


grid
spectrum = fft(y_n,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f_n=100*(0:511)/512;
figure
plot(f_n, norm_spectrum(1:512))
axis([0 max(f_n) 0 10])
grid

%y_1 = y_n;
y_n=conv(y_n,y_n);
for i=1:length(x_n)
    y_n(i)=y_n(i)/i;
end;
figure
plot(x_n(1:1000),y_n(1:1000));
grid