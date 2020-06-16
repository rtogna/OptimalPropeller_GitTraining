# GitFlow

Questa è una prova per capire il GitFlow e per utilizzare i comandi git.

<ol>
<li> Da GitHub: creare un fork del progetto per avere una propia repo del progetto.</li>
<li> Da terminale: clonare il progetto con il comando "git clone &#60URL&#62"</li>
<li> Da terminale: passare al branch develop con il comando "git checkout -b develop origin/develop"</li>
<li> Da terminale: creare un nuovo branch a partire da quello in cui ci troviamo (develop) con il comando "git checkout -b feature_NuovaFeature" (quindi il nuovo branch si chiamerà feature_NuovaFeature)</li>
<ol>
<li> Da GitHub: creare un nuovo branch con lo stesso nome di quello creato con git da terminale. DA VERIFICARE SE NECESSARIO!!!</li>
</ol>
<li> Adesso abbiamo creato questo nuovo branch e ci troviamo a lavorare su di esso. Si può fare un controllo per capire che stiamo lavorando in questo branch dando il comando "git status".</li>
<li> Lavoriamo sui file modificandoli con un IDE ad esempio.</li>
<li> Diamo di nuovo il comando "git status" per vedere lo stato dei file modificati. Ci verrà detto che non sono tracciate le modifiche perchè dobbiamo fare l'add del prossimo punto.</li>
<li> Comando "git add file1.cpp file2.cpp" significa che stiamo aggiungendo i file nominati file1.cpp e file2.cpp di cui tracciamo le modifiche, o se sono nuovi vengono aggiunti.</li>
<li> Comando "git commit -m "Mettere una breve descrizione della modifica per i file selezionati con il comando add"".</li>
<li> Comando "git push origin feature_NuovaFeature" per inviare le modifiche su GitHub.</li>
</ol>

<ul>
<li> Per passare da un branch ad un altro gia esistenti usare il comando "git checkout nomeBranch". </li>
<li>Ogni volta che dobbiamo fare nuove modifiche a dei file dobbiamo sempre assicurarci di essere sul branch giusto con "git status" e dobbiamo sempre fare prima un "git pull" nel caso in cui qualcun altro abbia modificato i file. </li>
</ul>
