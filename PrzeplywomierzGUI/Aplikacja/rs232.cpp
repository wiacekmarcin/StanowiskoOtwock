#include "rs232.h"
#if SYMULATOR
#else
#include <initguid.h>
#include <windows.h>
#include <Setupapi.h>
#include <winreg.h>
#include <wchar.h>
#endif
#include <string.h>


#include <QDebug>

#define RS232_PORTNR  32

#if SYMULATOR
#else
HANDLE Cport[RS232_PORTNR];
#endif

const char *comports[RS232_PORTNR]={"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                                    "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                                    "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                    "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
                                    "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
                                    "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
                                    "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
                                    "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};

char mode_str[128];

#include <QDebug>
int RS232_OpenComport(int comport_number, int baudrate, const char *mode, int flowctrl)
{
#if SYMULATOR
    (void)comport_number;
    (void)baudrate;
    (void)mode;
    (void)flowctrl;
    return 0;
#else

  if((comport_number>=RS232_PORTNR)||(comport_number<0))
  {

    return(1);
  }

  switch(baudrate)
  {
    case     110 : strcpy_s(mode_str, "baud=110");
                   break;
    case     300 : strcpy_s(mode_str, "baud=300");
                   break;
    case     600 : strcpy_s(mode_str, "baud=600");
                   break;
    case    1200 : strcpy_s(mode_str, "baud=1200");
                   break;
    case    2400 : strcpy_s(mode_str, "baud=2400");
                   break;
    case    4800 : strcpy_s(mode_str, "baud=4800");
                   break;
    case    9600 : strcpy_s(mode_str, "baud=9600");
                   break;
    case   19200 : strcpy_s(mode_str, "baud=19200");
                   break;
    case   38400 : strcpy_s(mode_str, "baud=38400");
                   break;
    case   57600 : strcpy_s(mode_str, "baud=57600");
                   break;
    case  115200 : strcpy_s(mode_str, "baud=115200");
                   break;
    case  128000 : strcpy_s(mode_str, "baud=128000");
                   break;
    case  256000 : strcpy_s(mode_str, "baud=256000");
                   break;
    case  500000 : strcpy_s(mode_str, "baud=500000");
                   break;
    case  921600 : strcpy_s(mode_str, "baud=921600");
                   break;
    case 1000000 : strcpy_s(mode_str, "baud=1000000");
                   break;
    case 1500000 : strcpy_s(mode_str, "baud=1500000");
                   break;
    case 2000000 : strcpy_s(mode_str, "baud=2000000");
                   break;
    case 3000000 : strcpy_s(mode_str, "baud=3000000");
                   break;
    default      : printf("invalid baudrate\n");
                   return(1);
                   break;
  }

  if(strlen(mode) != 3)
  {

    return(1);
  }

  switch(mode[0])
  {
    case '8': strcat_s(mode_str, " data=8");
              break;
    case '7': strcat_s(mode_str, " data=7");
              break;
    case '6': strcat_s(mode_str, " data=6");
              break;
    case '5': strcat_s(mode_str, " data=5");
              break;
    default : printf("invalid number of data-bits '%c'\n", mode[0]);
              return(1);
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': strcat_s(mode_str, " parity=n");
              break;
    case 'E':
    case 'e': strcat_s(mode_str, " parity=e");
              break;
    case 'O':
    case 'o': strcat_s(mode_str, " parity=o");
              break;
    default : printf("invalid parity '%c'\n", mode[1]);
              return(1);
              break;
  }

  switch(mode[2])
  {
    case '1': strcat_s(mode_str, " stop=1");
              break;
    case '2': strcat_s(mode_str, " stop=2");
              break;
    default : printf("invalid number of stop bits '%c'\n", mode[2]);
              return(1);
              break;
  }

  if(flowctrl)
  {
    strcat_s(mode_str, " xon=off to=off odsr=off dtr=on rts=off");
  }
  else
  {
    strcat_s(mode_str, " xon=off to=off odsr=off dtr=on rts=on");
  }

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363145%28v=vs.85%29.aspx

http://technet.microsoft.com/en-us/library/cc732236.aspx

https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-_dcb
*/

  Cport[comport_number] = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL /*| FILE_FLAG_OVERLAPPED,       */,                   /* no threads */
                      NULL);                      /* no templates */

  if(Cport[comport_number]==INVALID_HANDLE_VALUE)
  {



    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(mode_str, &port_settings))
  {

    CloseHandle(Cport[comport_number]);
    return(1);
  }

  if(flowctrl)
  {
    port_settings.fOutxCtsFlow = TRUE;
    port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
  }

  if(!SetCommState(Cport[comport_number], &port_settings))
  {

    CloseHandle(Cport[comport_number]);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport[comport_number], &Cptimeouts))
  {

    CloseHandle(Cport[comport_number]);
    return(1);
  }

  return(0);
#endif
}


int RS232_PollComport(int comport_number, unsigned char *buf, int size)
{
#if SYMULATOR
    (void)comport_number;
    (void)buf;
    (void)size;
    return 0;
#else
  int n;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  if(!ReadFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return -1;
  }

  return(n);
#endif
}


int RS232_SendByte(int comport_number, unsigned char byte)
{
#if SYMULATOR
    (void)comport_number;
    (void)byte;
    return 0;
#else
  int n;

  if(!WriteFile(Cport[comport_number], &byte, 1, (LPDWORD)((void *)&n), NULL))
  {
    return(1);
  }

  if(n<0)  return(1);

  return(0);
#endif
}


int RS232_SendBuf(int comport_number, unsigned char *buf, int size)
{
#if SYMULATOR
    (void)comport_number;
    (void)buf;
    (void)size;
    return 0;
#else
  int n;

  if(WriteFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
#endif
}


void RS232_CloseComport(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  CloseHandle(Cport[comport_number]);
#endif
}

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363258%28v=vs.85%29.aspx
*/

int RS232_IsDCDEnabled(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
    return 0;
#else
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_RLSD_ON) return(1);
  else return(0);
#endif
}


int RS232_IsRINGEnabled(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
    return 0;
#else
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_RING_ON) return(1);
  else return(0);
#endif
}


int RS232_IsCTSEnabled(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
    return 0;
#else
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_CTS_ON) return(1);
  else return(0);
#endif
}


int RS232_IsDSREnabled(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
    return 0;
#else
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_DSR_ON) return(1);
  else return(0);
#endif
}


void RS232_enableDTR(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  EscapeCommFunction(Cport[comport_number], SETDTR);
#endif
}


void RS232_disableDTR(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  EscapeCommFunction(Cport[comport_number], CLRDTR);
#endif
}


void RS232_enableRTS(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  EscapeCommFunction(Cport[comport_number], SETRTS);
#endif
}


void RS232_disableRTS(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  EscapeCommFunction(Cport[comport_number], CLRRTS);
#endif
}

/*
https://msdn.microsoft.com/en-us/library/windows/desktop/aa363428%28v=vs.85%29.aspx
*/

void RS232_flushRX(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  PurgeComm(Cport[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
#endif
}


void RS232_flushTX(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  PurgeComm(Cport[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
#endif
}


void RS232_flushRXTX(int comport_number)
{
#if SYMULATOR
    (void)comport_number;
#else
  PurgeComm(Cport[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
  PurgeComm(Cport[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
#endif
}

void RS232_cputs(int comport_number, const char *text)  /* sends a string to serial port */
{
#if SYMULATOR
    (void)comport_number;
    (void)text;
#else
  while(*text != 0)   RS232_SendByte(comport_number, *(text++));
#endif
}


/* return index in comports matching to device name or -1 if not found */
int RS232_GetPortnr(const char *devname)
{
#if SYMULATOR
    (void)devname;
    return 0;
#else
  int i;

  char str[32];

#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */
  strcpy_s(str, "/dev/");
#else  /* windows */
  strcpy_s(str, "\\\\.\\");
#endif
  strncat_s(str, devname, 16);
  str[31] = 0;

  for(i=0; i<RS232_PORTNR; i++)
  {
    if(!strcmp(comports[i], str))
    {
      return i;
    }
  }

  return -1;  /* device not found */
#endif
}

void GetComPortUsb(char *pszComePort, const char * vid, const char * pid)
{
#if SYMULATOR
    (void)pszComePort;
    (void)vid;
    (void)pid;
#else

    HDEVINFO DeviceInfoSet;
    DWORD DeviceIndex =0;
    SP_DEVINFO_DATA DeviceInfoData;
    char DevEnum[4] = {'U', 'S', 'B', 0};
    char ExpectedDeviceId[80] = {0}; //Store hardware id
    BYTE szBuffer[1024] = {0};
    DEVPROPTYPE ulPropertyType;
    DWORD dwSize = 0;
    DWORD Error = 0;
    //create device hardware id
    sprintf(ExpectedDeviceId,"VID_%s&PID_%s", vid, pid);
    //strcpy_s(ExpectedDeviceId, "vid_");
    //strcpy_s(ExpectedDeviceId + 4, vid);
    //strcpy_s(&ExpectedDeviceId[8], "&pid_");
    //strcpy_s(&ExpectedDeviceId[13], pid);

    //SetupDiGetClassDevs returns a handle to a device information set
    DeviceInfoSet = SetupDiGetClassDevs(
                        NULL,
                        (PCWSTR)DevEnum,
                        NULL,
                        DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (DeviceInfoSet == INVALID_HANDLE_VALUE) {

        return;
    }
    //Fills a block of memory with zeros
    ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    //Receive information about an enumerated device
    if(!SetupDiEnumDeviceInfo(
                DeviceInfoSet,
                DeviceIndex,
                &DeviceInfoData))
    {

    }
    while (SetupDiEnumDeviceInfo(
                DeviceInfoSet,
                DeviceIndex,
                &DeviceInfoData))
    {

        DeviceIndex++;
        //Retrieves a specified Plug and Play device property
        if (SetupDiGetDeviceRegistryProperty (DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID,
                                              &ulPropertyType, (BYTE*)szBuffer,
                                              sizeof(szBuffer),   // The size, in bytes
                                              &dwSize))
        {


            HKEY hDeviceRegistryKey;
            //Get the key
            hDeviceRegistryKey = SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData,DICS_FLAG_GLOBAL, 0,DIREG_DEV, KEY_READ);
            if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
            {
                Error = GetLastError();

                break; //Not able to open registry
            }
            else
            {
                // Read in the name of the port
                char pszPortName[32];
                DWORD dwSize = sizeof(pszPortName);
                DWORD dwType = 0;
                if( (RegQueryValueEx(hDeviceRegistryKey, L"PortName", NULL, &dwType, (LPBYTE) pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
                {
                    // Check if it really is a com port
                    if( _strnicmp( pszPortName, "COM", 3) == 0)
                    {
                        int nPortNr = atoi( pszPortName + 3 );
                        if( nPortNr != 0 )
                        {
                            strcpy_s(pszComePort,32,pszPortName);
                        }
                    }
                }
                // Close the key now that we are finished with it
                RegCloseKey(hDeviceRegistryKey);
            }
        }
    }
    if (DeviceInfoSet)
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    }
#endif
}
