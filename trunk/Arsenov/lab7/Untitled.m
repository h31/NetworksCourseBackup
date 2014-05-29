close all;
clear all;

x = 0:0.1:8*3.14;
f0 = 0.1;

% �������� ������
y = sin(2*pi*f0*x);
figure
plot(x(1:250),y(1:250))
grid

% ������ ��������� �������
spectrum = fft(y, 512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f = 100*(0:255)/512;
figure
plot(f, norm_spectrum(1:256))
axis([0 max(f) 0 70])
grid

% ����������� ���������
Fc = 10*f0;
Fs = 100*f0;
U = ammod(y, Fc, Fs, 0, 1);
figure 
plot(x(1:250), U(1:250))
grid

 % ������ ��������������� �������
 u_spectrum = fft(U, 512);
 norm_u_spectrum = u_spectrum.*conj(u_spectrum)/512;
 figure
 plot(f, norm_u_spectrum(1:256))
 axis([0 max(f) 0 70])
 grid

% ����������� ��������� � ����������� �������
Fc = 10*f0;
Fs = 100*f0;
U = ammod(y, Fc, Fs);
figure 
plot(x(1:250), U(1:250))
grid

% ������ ��������������� �������
u_spectrum = fft(U, 512);
norm_u_spectrum = u_spectrum.*conj(u_spectrum)/512;
figure
plot(f, norm_u_spectrum(1:256))
axis([0 max(f) 0 70])
grid

% ������������ ���������
Fc = 10*f0;
Fs = 100*f0;
U = ssbmod(y, Fc, Fs, [], 'upper');
figure 
plot(x(1:250), U(1:250))
grid

% ������ ��������������� �������
u_spectrum = fft(U, 512);
norm_u_spectrum = u_spectrum.*conj(u_spectrum)/512;
figure
plot(f, norm_u_spectrum(1:256))
axis([0 max(f) 0 70])
grid

%���������� ��������������
[b, a] = butter(10, Fc*2/Fs);
z = ssbdemod(U, Fc, Fs, 0, b, a);
figure
plot(x(1:250), z(1:250))
grid

% ������ ����������������� �������
du_spectrum = fft(U, 512);
norm_du_spectrum = du_spectrum.*conj(du_spectrum)/512;
figure
plot(f, norm_du_spectrum(1:256))
axis([0 max(f) 0 70])
grid

% ������� ��� ��������� 
M = 0.5;
n = M^2/(M^2+2)