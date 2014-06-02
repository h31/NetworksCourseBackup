Fs = 300;
t = [0:Fs]'/Fs; % ¬ектор моментов времени
% —оздаем модулирующий сигнал в виде суммы двух синусоид
x = sin(2*pi*t);
Fc = 80;                       % Ќесуща€ частота 
fdev = 30;               % ƒевиаци€ дл€ частотной модул€ции

figure
subplot(4, 1, 1);
plot(t, x)
grid
title('»сходный сигнал')

y = fmmod(x,Fc,Fs,fdev);   % „астотна€ модул€ци€

subplot(4, 1, 2);
plot(t, y)
grid
title('„астотна€ модул€ци€')

spectrum = fft(y, 512);      % —пектр частотной модул€ции
norm_spectrum = spectrum.*conj(spectrum)/512;
f = 100*(0:255)/512;
subplot(4, 1, 3);
plot(f, norm_spectrum(1:256))
grid
title('—пектр частотной модул€ции')
axis([min(f) max(f) 0 max(norm_spectrum)]);

z = fmdemod(y,Fc,Fs,fdev); % ƒемодул€ци€
subplot(4, 1, 4);
plot(t, z)
grid
title('„астотна€ демодул€ци€')
