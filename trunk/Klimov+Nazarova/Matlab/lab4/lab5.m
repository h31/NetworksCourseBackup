% f = 10;
% x = 0:1/(2*pi):8*pi;%4 periods
% y = cos(2*pi*f*x);
% plot(x,y)
% grid
% figure
% spectrum = fft(y,512);
% spectrum = abs(fft(y,512));
% norm_spectrum = spectrum.*conj(spectrum)/512;%������ ��� �� ��� ����������� ����������� - ������ ������
% f=100*(0:511)/512;
% plot(f, norm_spectrum(1:512))
% axis([0 max(f) 0 10])
% grid
t = 0:0.1:4*pi;
N=100;
y = 0;
for i=0:N
    y = y + cos(i*t);
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