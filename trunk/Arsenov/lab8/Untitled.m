x = 0:0.01:4*pi;
y = sin(4*pi*x);

figure
subplot(4, 1, 1);
plot(x(1:500), y(1:500))
grid
title('»сходный сигнал')
 

Fs = 2000;                      % „астота дискретизации 
Fc = 8;                         % Ќесуща€ частота 
dev1 = pi/2;                % ƒевиаци€ фазы дл€ фазовой модул€ции
pm_y = pmmod(x,Fc,Fs,dev1); % ‘азова€ модул€ци€
subplot(4, 1, 2);
plot(x(1:500), pm_y(1:500))
grid
title('‘азова€ модул€ци€')
spectrum = fft(pm_y, 512);      % —пектр фазовой модул€ции
norm_spectrum = spectrum.*conj(spectrum)/512;  
f = 100*(0:255)/512;
subplot(4, 1, 3);
plot(f, norm_spectrum(1:256))
grid
title('—пектр фазовой модул€ции')
axis([min(f) max(f) 0 max(norm_spectrum)]);
pdm_y = pmdemod(pm_y,Fc,Fs,dev1); % ƒемодул€ци€
subplot(4, 1, 4);
plot(x, pdm_y)
grid
title('‘азова€ демодул€ци€')

figure
subplot(4, 1, 1);
plot(x(1:500), y(1:500))
grid
title('»сходный сигнал')


dev2 = 20;                
fm_y = fmmod(x,Fc,Fs,dev2); 
subplot(4, 1, 2);
plot(x(1:500), fm_y(1:500))
grid
title('„астотна€ модул€ци€')
spectrum = fft(fm_y, 512);     
norm_spectrum = spectrum.*conj(spectrum)/512;  
f = 100*(0:255)/512;
subplot(4, 1, 3);
plot(f, norm_spectrum(1:256))
grid
title('—пектр частотной модул€ции')
axis([min(f) max(f) 0 max(norm_spectrum)]);
fdm_y = fmdemod(fm_y,Fc,Fs,dev2); 
subplot(4, 1, 4);
plot(x, fdm_y)
grid
title('„астотна€ демодул€ци€')
