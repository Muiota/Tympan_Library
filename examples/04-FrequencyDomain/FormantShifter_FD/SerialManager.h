

#ifndef _SerialManager_h
#define _SerialManager_h

#include <Tympan_Library.h>


//now, define the Serial Manager class
class SerialManager {
  public:
  public:
    SerialManager(TympanBase &_audioHardware)
      : audioHardware(_audioHardware)
    {  };
    //SerialManager(void)
    //{  };
          
    void respondToByte(char c);
    void printHelp(void);
    int N_CHAN;
    float channelGainIncrement_dB = 2.5f; 
    float formantScaleIncrement = powf(2.0,1.0/6.0);
    
  private:
    TympanBase &audioHardware;
};
#define thisSerial audioHardware

void SerialManager::printHelp(void) {  
  thisSerial.println();
  thisSerial.println("SerialManager Help: Available Commands:");
  thisSerial.println("   h: Print this help");
  thisSerial.println("   g: Print the gain settings of the device.");
  thisSerial.println("   C: Toggle printing of CPU and Memory usage");
  thisSerial.println("   p: Switch to built-in PCB microphones");
  thisSerial.println("   m: switch to external mic via mic jack");
  thisSerial.println("   l: switch to line-in via mic jack");
  thisSerial.print("   k: Increase the gain of all channels (ie, knob gain) by "); thisSerial.print(channelGainIncrement_dB); thisSerial.println(" dB");
  thisSerial.print("   K: Decrease the gain of all channels (ie, knob gain) by ");thisSerial.print(-channelGainIncrement_dB); thisSerial.println(" dB");
  thisSerial.print("   f: Raise formant shifting (change by "); thisSerial.print(formantScaleIncrement); thisSerial.println("x)");
  thisSerial.print("   F: Lower formant shifting (change by "); thisSerial.print(1.0/formantScaleIncrement); thisSerial.println("x)");  thisSerial.println();
}

//functions in the main sketch that I want to call from here
extern void incrementKnobGain(float);
extern void printGainSettings(void);
extern void togglePrintMemoryAndCPU(void);
extern float incrementFormantShift(float);
extern void switchToPCBMics(void);
extern void swtichToMicInOnMicJack(void);
extern void switchToLineInOnMicJack(void);

//switch yard to determine the desired action
void SerialManager::respondToByte(char c) {
  //float old_val = 0.0, new_val = 0.0;
  switch (c) {
    case 'h': case '?':
      printHelp(); break;
    case 'g': case 'G':
      printGainSettings(); break;
    case 'k':
      incrementKnobGain(channelGainIncrement_dB); break;
    case 'K':   //which is "shift k"
      incrementKnobGain(-channelGainIncrement_dB);  break;
    case 'C': case 'c':
      thisSerial.println("Received: toggle printing of memory and CPU usage.");
      togglePrintMemoryAndCPU(); break;
    case 'p':
      switchToPCBMics(); break;
    case 'm':
      swtichToMicInOnMicJack(); break;
    case 'l':
      switchToLineInOnMicJack();break;
    case 'f':
      { float new_val = incrementFormantShift(formantScaleIncrement);
      thisSerial.print("Recieved: new format scale = "); thisSerial.println(new_val);}
      break;
    case 'F':
      { float new_val = incrementFormantShift(1./formantScaleIncrement);
      thisSerial.print("Recieved: new format scale = "); thisSerial.println(new_val);}
      break;
  }
}


#endif
