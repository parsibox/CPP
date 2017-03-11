pkill -9 Publisher.
pkill -9 Recorder
pkill -9 UssdClient


nohup ./HttpPublisher.exe >/dev/null &
nohup ./Recorder.exe >/dev/null &
nohup ./UssdClient.exe >/dev/null &
