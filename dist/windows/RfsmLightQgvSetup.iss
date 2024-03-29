[Setup]
AppId={{5D1BF4DD-8593-48B4-96CA-0225513733D7}
AppName=RfsmLight
AppVersion=1.3.1
AppContact=jocelyn.serot@uca.fr
AppCopyright=Copyright (C) 2019-2021 J. Serot
AppPublisher=J. Serot / Clermont-Auvergne University
AppPublisherURL=https://github.com/jserot/rfsm-light
LicenseFile=Z:/Qt/rfsm-light/LICENSE
UsePreviousAppDir=false
DefaultDirName={pf}/RfsmLight
DefaultGroupName=RfsmLight
Compression=none
;Compression=lzma2
;SolidCompression=yes
OutputBaseFilename=RfsmLight-1.3.1-qgv_setup
OutputDir=.
WizardImageFile=Z:\Qt\rfsm-light\build\rfsm.bmp
WizardSmallImageFile=Z:\Qt\rfsm-light\build\rfsm_small.bmp

[Files]
Source: "Z:\Qt\rfsm-light\build\*"; DestDir: "{app}"; Excludes:"examples"; Flags: recursesubdirs
Source: "Z:\Qt\rfsm-light\build\examples\*"; DestDir: "{code:GetDirs|0}\RfsmLightExamples"; Flags: recursesubdirs
 
[Icons]
Name: "{group}\RfsmLight"; Filename: "{app}\RfsmLight.exe";IconFilename: "Z:\Qt\rfsm-light\build\rfsm.ico"
Name: "{group}\{cm:UninstallProgram,RfsmLight}"; Filename: "{uninstallexe}";IconFilename: "Z:\Qt\rfsm-light\build\rfsmun.ico"
Name: "{commondesktop}\RfsmLight"; Filename: "{app}\RfsmLight.exe";    IconFilename: "Z:\Qt\rfsm-light\build\rfsm.ico"

[Run]
Filename: "{app}\rfsm-light.exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

[INI]
Filename: "{app}\rfsm-light.ini"; Section: "Settings"; Flags: uninsdeletesection
Filename: "{app}\rfsm-light.ini"; Section: "Settings"; Key: "COMPILER"; String: "{app}\rfsmc"
Filename: "{app}\rfsm-light.ini"; Section: "Settings"; Key: "VCDVIEWER"; String: "{code:GetFiles|0}"
Filename: "{app}\rfsm-light.ini"; Section: "Settings"; Key: "INITDIR"; String: "{code:GetDirs|0}\RfsmLightExamples"

[UninstallDelete]
Type: files; Name: "{app}\rfsm-light.ini"

[Code]
var
  DirPage: TInputDirWizardPage;
  FilePage: TInputFileWizardPage;
  VcdViewerLocation: String;

function GetDirs(Param: String): String;
begin
  Result := DirPage.Values[StrToInt(Param)];
end;

function GetFiles(Param: String): String;
begin
  Result := FilePage.Values[StrToInt(Param)];
end;

procedure InitializeWizard;
begin
  { Directory input page for specifying where to store examples }
  DirPage := CreateInputDirPage(
    wpSelectDir,
    'Select Destination Location for the directory containing the examples',
    'Please choose a directory with read-write permissions',
    'Setup will install examples in the following directory',
    True,
    '');
  DirPage.Add('');
  DirPage.Values[0] := GetPreviousData('Directory1', ExpandConstant('{userdocs}'));
  { File input page for specifying auxilliary programs }
  FilePage := CreateInputFilePage(
    wpSelectDir,
   'Select Locations of GTKWAVE program',
   '',
   'This program is required to ' + #13#10
   + '- view the execution traces generated by the simulator' + #13#10
   + 'It can be downloaded from gtkwave.sourceforge.net respectively.');
  FilePage.Add('GTKWAVE:', 'Executable files|*.exe|All files|*.*', '.exe');
  FilePage.Values[0] := ExpandConstant('{pf}\Gtkwave\bin\gtkwave.exe'); 
  VcdViewerLocation := FilePage.Values[0];
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  { store chosen directories for the next run of the setup }
  SetPreviousData(PreviousDataKey, 'Directory1', DirPage.Values[0]);
  SetPreviousData(PreviousDataKey, 'FileLocation3', FilePage.Values[0]);
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID=wpReady then
  begin
   Wizardform.ReadyMemo.Lines.Add(''); 
   Wizardform.ReadyMemo.Lines.Add('Directory containing the examples:');
   Wizardform.ReadyMemo.Lines.Add('    ' + DirPage.Values[0] + '\RfsmLightExamples');
   Wizardform.ReadyMemo.Lines.Add('');
   Wizardform.ReadyMemo.Lines.Add('Program for viewing .vcd files:');
   Wizardform.ReadyMemo.Lines.Add('    ' + FilePage.Values[0]);
 end;
end;
