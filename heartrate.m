clc
close all
clear all

Data = csvread('HW6Vals.csv', 1, 0);
%Data = Data(1:528,:);
SampleFreq = 50;%30000/(80*60);
SampleTime = 1/SampleFreq;
IR = Data(:,1);
RED = Data(:,2);
Time = [0:SampleTime:(size(IR)-1)*SampleTime];

plot(Time, IR, 'b', Time, RED, 'r');
title('IR and RED signals against Time');

%%
FilterDegree = 4; LowFreq = 1; HighFreq = 2.0; SampleFreq = 50; FirstSample = FilterDegree*SampleFreq;
hd = design(fdesign.bandpass('N,Fc1,Fc2',FilterDegree,LowFreq,HighFreq,SampleFreq),'butter');
fvtool(hd)

IRFiltered = filter(hd,IR);
REDFiltered = filter(hd,RED);

figure
plot(Time(FirstSample:end), IRFiltered(FirstSample:end), 'b', Time(FirstSample:end), REDFiltered(FirstSample:end), 'r');
title('Filtered IR and RED signals');
%%
SignalToUse = IRFiltered(FirstSample:end);
AvgWindow = 265;
AvgFilter = ones(1,AvgWindow)/AvgWindow;

SignalToUse = filter(AvgFilter, 1, SignalToUse);
%%
Threshold = 0.0;
MinPeakHeight = 1;
MinPeakDistance = 2;
figure, plot(Time(FirstSample:end), SignalToUse), hold on

[peakValues, indexes] = findpeaks(SignalToUse, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);

plot(Time(indexes) + FilterDegree, peakValues + 0.05, 'k^', 'markerfacecolor', [1 0 0])
title('Signal with detected peaks');

%%
HeartRate = zeros(size(indexes));
Window = 265;
if Window < 2 
    Window = 2;
end

LoopStart = Window;

for i= LoopStart : 1 : size(indexes)
    
    for j = i-Window+1 : 1 : i-1
        HeartRate(i) = HeartRate(i) + (60 * SampleFreq) / (indexes(j+1) - indexes(j));
    end
    
    HeartRate(i) = HeartRate(i) / (Window - 1);
    
end

HeartRate = HeartRate(LoopStart:size(HeartRate));

figure, plot(HeartRate)
title('Detected Heart Rate');
DataToWrite = zeros(size(IR));
DataToWrite(1:max(size(HeartRate))) = HeartRate;
DataToWrite = [Data DataToWrite];
dlmwrite('HW6Vals.csv',DataToWrite,'delimiter',',');
