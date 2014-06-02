Fs = 300;
t = [0:Fs]'/Fs; % ������ �������� �������
% ������� ������������ ������ � ���� ����� ���� ��������
x = sin(2*pi*t);
Fc = 80;                       % ������� ������� 
fdev = 30;               % �������� ��� ��������� ���������

figure
subplot(4, 1, 1);
plot(t, x)
grid
title('�������� ������')

y = fmmod(x,Fc,Fs,fdev);   % ��������� ���������

subplot(4, 1, 2);
plot(t, y)
grid
title('��������� ���������')

spectrum = fft(y, 512);      % ������ ��������� ���������
norm_spectrum = spectrum.*conj(spectrum)/512;
f = 100*(0:255)/512;
subplot(4, 1, 3);
plot(f, norm_spectrum(1:256))
grid
title('������ ��������� ���������')
axis([min(f) max(f) 0 max(norm_spectrum)]);

z = fmdemod(y,Fc,Fs,fdev); % �����������
subplot(4, 1, 4);
plot(t, z)
grid
title('��������� �����������')
