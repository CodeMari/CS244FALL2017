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
AvgWindow = 151;
AvgFilter = ones(1,AvgWindow)/AvgWindow;

IRFiltered = filter(AvgFilter, 1, IR);
REDFiltered = filter(AvgFilter, 1, RED);

IRFiltered = IRFiltered(AvgWindow:end);
REDFiltered = REDFiltered(AvgWindow:end);
Time = Time(AvgWindow:end);

figure,
plot(Time, IRFiltered, 'b', Time, REDFiltered, 'r')
title('Filtered IR and RED signals against Time');
hold on

%%

Threshold = 0.0;
MinPeakHeight = 1;
MinPeakDistance = 150;

[IRPeakValues, IRPeakIndexes] = findpeaks(IRFiltered, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);
[REDPeakValues, REDPeakIndexes] = findpeaks(REDFiltered, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);
plot(Time(IRPeakIndexes ), IRPeakValues + 0.05, 'k^', 'markerfacecolor', [1 0 0])
hold on
plot(Time(REDPeakIndexes ), REDPeakValues + 0.05, 'k^', 'markerfacecolor', [0 0 1])
hold on
title('IR and RED Signals with detected peaks');

%%

FlipIRFiltered = -1*IRFiltered + 2*mean(IRFiltered);
FlipREDFiltered = -1*REDFiltered + 2*mean(REDFiltered);

[IRValleyValues, IRValleyIndexes] = findpeaks(FlipIRFiltered, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);
[REDValleyValues, REDValleyIndexes] = findpeaks(FlipREDFiltered, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);

IRValleyValues = -1*IRValleyValues + 2*mean(IRFiltered);
REDValleyValues = -1*REDValleyValues + 2*mean(REDFiltered);

plot(Time(IRValleyIndexes), IRValleyValues + 0.05, 'kv', 'markerfacecolor', [0 1 1])
hold on
plot(Time(REDValleyIndexes), REDValleyValues + 0.05, 'kv', 'markerfacecolor', [1 1 0])
title('Signal with detected peaks');
%%

Window = 101;
LastValleyIndex = 0;
IRSize = size(IRPeakIndexes) - Window + 1;
IRSize = IRSize(1);
IRDC = zeros(IRSize,1);
IRAC = zeros(IRSize,1);

for i = 1 : size(IRPeakIndexes) - Window + 1
    
    IRDCTemp = 0; IRACTemp = 0;
    
    for j = i : i + Window - 1
        
        PeakIRIndex = IRPeakIndexes(j);
        IRPeakTime = Time(PeakIRIndex);
        
        for k = LastValleyIndex + 1 : size(IRValleyIndexes)
            ValleyIRIndex = IRValleyIndexes(k);
            IRValleyTime = Time(ValleyIRIndex);
            
            if(IRValleyTime > IRPeakTime)
                break;
            end
            
        end
        
        if (IRValleyTime - IRPeakTime <= 0.1)
            break;
        end
        
        IRDCTemp = IRDCTemp + min(IRFiltered(PeakIRIndex), IRFiltered(ValleyIRIndex));
        IRACTemp = IRACTemp + (IRFiltered(PeakIRIndex) - IRFiltered(ValleyIRIndex));
        
    end
    
    IRDC(i) = IRDCTemp / (Window );
    IRAC(i) = IRACTemp / (Window );
    
end

IRRatio = IRAC./IRDC;

%%

LastValleyIndex = 0;
REDSize = size(REDPeakIndexes) - Window + 1;
REDSize = REDSize(1);
REDDC = zeros(REDSize,1);
REDAC = zeros(REDSize,1);

for i = 1 : size(REDPeakIndexes) - Window + 1
    
    REDDCTemp = 0; REDACTemp = 0;
    
    for j = i : i + Window - 1
        
        PeakREDIndex = REDPeakIndexes(j);
        REDPeakTime = Time(PeakREDIndex);
        
        for k = LastValleyIndex + 1 : size(REDValleyIndexes)
            ValleyREDIndex = REDValleyIndexes(k);
            REDValleyTime = Time(ValleyREDIndex);
            
            if(REDValleyTime > REDPeakTime)
                break;
            end
            
        end
        
        if (REDValleyTime - REDPeakTime <= 0.1)
            break;
        end
        
        REDDCTemp = REDDCTemp + min(REDFiltered(PeakREDIndex), REDFiltered(ValleyREDIndex));
        REDACTemp = REDACTemp + (REDFiltered(PeakREDIndex) - REDFiltered(ValleyREDIndex));
        
    end
    
    REDDC(i) = REDDCTemp / (Window );
    REDAC(i) = REDACTemp / (Window );
    
end

REDRatio = REDAC./REDDC;

%%
MinSize = min(size(REDRatio),size(IRRatio));
Ratio = REDRatio(1:MinSize(1))./IRRatio(1:MinSize(1));

SPO2Vals = (-45.060*(Ratio.^2)/10000) + 30.354 * Ratio/100 + 94.845 ;
SPO2Vals(isnan(SPO2Vals)) = [];
figure,
plot(SPO2Vals)
title('Percentage Saturation')

DataToWrite = zeros(size(IR));
DataToWrite(1:max(size(SPO2Vals))) = SPO2Vals;
DataToWrite = [Data DataToWrite];
dlmwrite('HW6Vals.csv',DataToWrite,'delimiter',',');
