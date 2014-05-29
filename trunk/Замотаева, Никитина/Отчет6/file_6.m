x = 0:0.01:4*pi;
f = 100*(0:255)/512;

figure
noise = rand(size(x));
%������������� ������
y = sin(2*pi*x);
%������ � �����
y_noisy = y + 0.3*noise;
plot(x(1:200),y_noisy(1:200))
grid

%������ ��� �����������
[B,A] = butter(16,0.98); 
B=B./sum(B);
A=A./sum(A);

%��������� ������� ���
figure
y_filtered = conv(y_noisy,[B,A]);
plot(x(1:200),y_filtered(1:200))
grid

%������ ��������� ������� � �����
figure
noisy_spectrum = fft(y_noisy,512);
norm_noisy_spectrum = noisy_spectrum.*conj(noisy_spectrum)/512;
plot(f,norm_noisy_spectrum(1:256))
axis([0 max(f) 0 2])
grid

%������ ���������������� �������
figure
spectrum = fft(y_filtered,512);
norm_filtered_spectrum = spectrum.*conj(spectrum)/512;
plot(f,norm_filtered_spectrum(1:256))
axis([0 max(f) 0 2])
grid