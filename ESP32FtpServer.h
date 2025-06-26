#ifndef FTP_SERVERESP_H
#define FTP_SERVERESP_H

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <SD_MMC.h>
#include <functional>
#include <map>

#define FTP_CTRL_PORT        21
#define FTP_DATA_PORT_PASV   50009
#define FTP_CMD_SIZE         263
#define FTP_CWD_SIZE         263
#define FTP_FIL_SIZE         255
#define FTP_BUF_SIZE         4096
#define FTP_TIME_OUT 5  // Tempo de inatividade em minutos amauri
#define FTP_SERVER_VERSION "FTP-2025-06-26"

enum FileSystemType { FILESYSTEM_SD, FILESYSTEM_SPIFFS, FILESYSTEM_LITTLEFS };

class FtpServer {
public:
  void begin(String uname, String pword, FS* fs, FileSystemType fsType);
  void handleFTP();
  void setTimeout(uint32_t minutes);
  void onUpload(std::function<void(const String&)> callback);
  void onDownload(std::function<void(const String&)> callback);
  void addCustomCommand(const String& cmd, std::function<void(const String&)> handler);
  void debugStatus();

private:
  void    iniVariables();
  void    clientConnected();
  void    disconnectClient();
  bool    userIdentity();
  bool    userPassword();
  bool    processCommand();
  bool    dataConnect();
  bool    doRetrieve();
  bool    doStore();
  void    closeTransfer();
  void    abortTransfer();
  bool    makePath(char* fullname);
  bool    makePath(char* fullName, char* param);
  bool    makeExistsPath(char* path, char* param = nullptr);
  bool haveParameter();
  bool formatLIST(File* file, char* buffer);
  bool formatMLST(File* file, char* buffer);


  uint8_t getDateTime(uint16_t* pyear, uint8_t* pmonth, uint8_t* pday,
                      uint8_t* phour, uint8_t* pminute, uint8_t* second);
  char*   makeDateTimeStr(char* tstr, uint16_t date, uint16_t time);
  int8_t  readChar();

  IPAddress dataIp;
  WiFiClient client;
  WiFiClient data;
  WiFiClientSecure secureClient;

  FileSystemType _fsType;
  FS* fileSystem;

  File file;
  bool dataPassiveConn;
  uint16_t dataPort;
  char buf[FTP_BUF_SIZE];
  char cmdLine[FTP_CMD_SIZE];
  char cwdName[FTP_CWD_SIZE];
  char command[5];
  bool rnfrCmd;
  char* parameters;
  uint16_t iCL;
  int8_t cmdStatus, transferStatus;
  uint32_t millisTimeOut, millisDelay, millisEndConnection, millisBeginTrans, bytesTransfered;
  String _FTP_USER, _FTP_PASS;

  std::function<void(const String&)> uploadCallback;
  std::function<void(const String&)> downloadCallback;
  std::map<String, std::function<void(const String&)>> customCommands;
};

#endif
