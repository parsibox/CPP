pkill -9 Publisher.
pkill -9 Recorder
pkill -9 XmlClient


nohup ./Publisher.exe >/dev/null &
nohup ./Recorder.exe >/dev/null &
nohup ./XmlClient.exe >/dev/null &
