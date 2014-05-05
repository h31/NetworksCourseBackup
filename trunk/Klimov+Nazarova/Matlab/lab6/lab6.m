x = 0:0.01:4*pi;
f = 100*(0:255)/512;

noise = rand(size(x));
y = sin(2*pi*x);            % ������ ��� ����
y_noisy = y+0.3*noise;      % ������ � �����

figure
plot(x(1:200),y(1:200))
grid

figure
plot(x(1:200),y_noisy(1:200))
grid

[B,A] = butter(16,0.98);    % ������ ��� �����������
B = B./sum(B);
A = A./sum(A);
                      
y_filtered = conv(y_noisy, [B, A]); % ��������� ������� ���

figure
plot(x(1:200),y_filtered(1:200))
grid

noisy_spectrum = fft(y_noisy, 512); % ������ ������� � �����
norm_noisy_spectrum = noisy_spectrum.*conj(noisy_spectrum)/512;

figure
plot(f, norm_noisy_spectrum(1:256))
axis([0 max(f) 0 2])
grid 

filtered_spectrum = fft(y_filtered, 512);   % ������ �������������� �������
norm_filtered_spectrum = filtered_spectrum.*conj(filtered_spectrum)/512;

figure
plot(f, norm_filtered_spectrum(1:256))
axis([0 max(f) 0 2])
grid
