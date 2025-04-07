Run it: ```apt install g++ geany```. Open the .cpp in Geany. Hit F9 once. F5 to run.

<br>

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/StatMerge/refs/heads/main/Other/Terminal.png">
</p>

<br>
<br>

## Top-occurring Bytes are final, else the smallest top-occurring Bytes

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/StatMerge/main/Other/What_it_does_b53a4b1452da928659635b21abd83f30418c241ee2af94802f0f30e151a99787.png">
</p>

Merges bytes up to smallest file (merges until "end of file" is reached in any file.)

<br>
<br>

## Merge PyTorch models (.pth)

That's right, you can merge PyTorch models.
Preferably, merge models that are copies of one but trained in any way,
or merge models that are snapshots of a model being updated forever.
(This is fine because PyTorch model training is nondeterministic by default.)
Make sure PyTorch creates models with names of equal length,
else model size differs.
