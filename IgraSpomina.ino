#define b3 A0
#define b6 A2
#define b9 A5
int stcp = 4;
int shcp = 3;
int ds = 2;

int BUZZ = 5;

int b1 = 8;
int b2 = 9;
int b4 = 10;
int b5 = 11;
int b7 = 12;
int b8 = 13;
int devet = 7;

int zamik = 600;
int ponavljaj = 3;
int sequence[10];

void setup() {
    Serial.begin(9600);
    pinMode(stcp, OUTPUT);
    pinMode(shcp, OUTPUT);
    pinMode(ds, OUTPUT);
    pinMode(BUZZ, OUTPUT);
    pinMode(devet, OUTPUT);
    pinMode(b1, INPUT_PULLUP);
    pinMode(b2, INPUT_PULLUP);
    pinMode(b3, INPUT_PULLUP);
    pinMode(b4, INPUT_PULLUP);
    pinMode(b5, INPUT_PULLUP);
    pinMode(b6, INPUT_PULLUP);
    pinMode(b7, INPUT_PULLUP);
    pinMode(b8, INPUT_PULLUP);
    pinMode(b9, INPUT_PULLUP);

    snakeAnimation();
}

void loop() {

    // Ob spremembi hitrosti prižiga LED diod, nastavi dolžino zaporedja na 3
    ponavljaj = 3;
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < ponavljaj; k++) {

            // Generiraj naključno število in jo shrani v array, na k-to mesto.
            int randomButton = random(1, 10);
            sequence[k] = randomButton;
            posveti(randomButton, true);

            // Brez zamika v zadnji ponovitvi zanke 
            // (uporabnik lahko takoj po končanem zaporedju začne vnašati zaporedje)
            if (k != ponavljaj - 1) {
                delay(zamik);
            }
        }

        // Funkcija, ki čaka na vnos zaporedja uporabnika (vrne 1 če se uporabnik zmoti, drugače 0)
        int checkIfWrong = gumb(ponavljaj);

        // Če se uporabik zmoti, ponastavi vrednosti in ponovno zaženi igro
        if (checkIfWrong == 1) {
            i = -1;
            ponavljaj = 3;
            zamik = 600;
            sequence[10];
            setup();
        } 
        // Če uporabnik uspešno vnese zaporedje, povečaj število utripov zaporedja
            ponavljaj = ponavljaj + 1;
    }
    // Ko se zgodi level-up, zmanjšaj zakasnitev med prižiganjem/ugašanjem ledic
    zamik = zamik - 200;

    // Ko je zamik 0, je konec igre
    if (zamik == 0) {

       digitalWrite(stcp, LOW);
        shiftOut(ds, shcp, MSBFIRST, 0B11111111);
        digitalWrite(stcp, HIGH);
        digitalWrite(devet, HIGH);

        playWinningMelody();

        digitalWrite(devet, LOW);
        digitalWrite(stcp, LOW);
        shiftOut(ds, shcp, MSBFIRST, 0B00000000);
        digitalWrite(stcp, HIGH);

        ponavljaj = 3;
        setup();
    }
}

int gumb(int in) {
    // Zanka, ki se ponovi za vsak element zaporedja
    for (int i = 0; i < in; i++) {

        // Čakaj na pritisk gumba
        while (digitalRead(b1) == HIGH && digitalRead(b2) == HIGH && digitalRead(b3) == HIGH &&
               digitalRead(b4) == HIGH && digitalRead(b5) == HIGH && digitalRead(b6) == HIGH &&
               digitalRead(b7) == HIGH && digitalRead(b8) == HIGH && digitalRead(b9) == HIGH) {}

        int pressedButton = 0;

        // Zapomni si gumb, ki ga je pritisnil uporabnik
        if (digitalRead(b1) == LOW) pressedButton = 1;
        else if (digitalRead(b2) == LOW) pressedButton = 2;
        else if (digitalRead(b3) == LOW) pressedButton = 3;
        else if (digitalRead(b4) == LOW) pressedButton = 4;
        else if (digitalRead(b5) == LOW) pressedButton = 5;
        else if (digitalRead(b6) == LOW) pressedButton = 6;
        else if (digitalRead(b7) == LOW) pressedButton = 7;
        else if (digitalRead(b8) == LOW) pressedButton = 8;
        else if (digitalRead(b9) == LOW) pressedButton = 9;

        // Preveri, če je pritisnjen gumb pravilen
        if (pressedButton != sequence[i]) {
            
            // Zaigraj tone piskača, ki ponazarjajo izgubo
            // Utripaj LED diode med igranjem piskača
            for (int j = 0; j < 5; j++) {
                digitalWrite(stcp, LOW);
                shiftOut(ds, shcp, MSBFIRST, 0B11111111);
                digitalWrite(stcp, HIGH);
                digitalWrite(devet, HIGH);

                if (j == 0) tone(BUZZ, 1000);
                else if (j == 1) tone(BUZZ, 800);
                else if (j == 2) tone(BUZZ, 600);
                else if (j == 3) tone(BUZZ, 400);
                else if (j == 4) tone(BUZZ, 200);

                delay(300);

                digitalWrite(stcp, LOW);
                shiftOut(ds, shcp, MSBFIRST, 0B00000000);
                digitalWrite(stcp, HIGH);
                digitalWrite(devet, LOW);

                delay(200);
            }

            noTone(BUZZ);
            delay(1000);

            return 1;
        }

        // Gumb je pravilen, posveti LED diodo, katero predstavlja izbran gumb.
        int temp = zamik;
        zamik = 450;
        posveti(pressedButton, true);
        zamik = temp;
    }
    // Posveti vse LED diode, kar ponazarja, da je vneseno zaporedje pravilno
    // in da prihaja novo zaporedje.
    flash(1000, 1);
    delay(1000);
    return 0;
}

void posveti(int number, bool buzz) {
    switch (number) {
        case 1:
            push(500, 0B00000001, buzz);
            break;
        case 2:
            push(600, 0B00000010, buzz);
            break;
        case 3:
            push(700, 0B00000100, buzz);
            break;
        case 4:
            push(800, 0B00001000, buzz);
            break;
        case 5:
            push(900, 0B00010000, buzz);
            break;
        case 6:
            push(1000, 0B00100000, buzz);
            break;
        case 7:
            push(1100, 0B01000000, buzz);
            break;
        case 8:
            push(1200, 0B10000000, buzz);
            break;
        case 9:
            digitalWrite(stcp, LOW);
            shiftOut(ds, shcp, MSBFIRST, 0B00000000);
            digitalWrite(stcp, HIGH);
            digitalWrite(devet, HIGH);
            if (buzz) {
                tone(BUZZ, 1300);
            }
            delay(zamik);
            digitalWrite(devet, LOW);
            if (buzz) {
                noTone(BUZZ);
            }
            break;
        default:
            push(200, 0B11111111, buzz);
            break;
    }
}

void push(int buzz, int byte, bool sound) {
    digitalWrite(stcp, LOW);
    shiftOut(ds, shcp, MSBFIRST, byte);
    digitalWrite(stcp, HIGH);

    if (sound) {
        tone(BUZZ, buzz);
    }
    delay(zamik);
    if (sound) {
        noTone(BUZZ);
    }

    digitalWrite(stcp, LOW);
    shiftOut(ds, shcp, MSBFIRST, 0B00000000);
    digitalWrite(stcp, HIGH);
}

void flash(int del, int ponovi) {
    for (int i = 0; i < ponovi; i++) {
        digitalWrite(stcp, LOW);
        shiftOut(ds, shcp, MSBFIRST, 0B11111111);
        digitalWrite(stcp, HIGH);
        digitalWrite(devet, HIGH);

        delay(del);

        digitalWrite(devet, LOW);
        digitalWrite(stcp, LOW);
        shiftOut(ds, shcp, MSBFIRST, 0B00000000);
        digitalWrite(stcp, HIGH);

        delay(del);
    }
}

void snakeAnimation() {

  // Definicija zaporedja LED diod, ki se bodo posamično prižigale.
    int zaporedje[] = {1, 2, 3, 6, 9, 8, 7, 4};
    zamik = 50;
  
    // Iteriraj skozi zaporedje, dokler uporabnik ne izbere nekega gumba.
    while (true) {
        for (int i = 0; i < 8; i++) {
            posveti(zaporedje[i], false);

            if (digitalRead(b1) == LOW || digitalRead(b2) == LOW || digitalRead(b3) == LOW ||
                digitalRead(b4) == LOW || digitalRead(b5) == LOW || digitalRead(b6) == LOW ||
                digitalRead(b7) == LOW || digitalRead(b8) == LOW || digitalRead(b9) == LOW) 
                {
                  // Nastavi zamik na začetno vrednost in se vrni v Setup().
                  zamik = 600;
                  delay(zamik);
                  return;
            }

            delay(25);
        }
    }
}

void playWinningMelody() {
    const int C = 261; 
    const int D = 294; 
    const int E = 329; 
    const int F = 349; 
    const int G = 392; 
    const int A = 440; 
    const int B = 493; 
    const int Fsharp = 370;

    int melody[] = {C, E, G, C, G, E, C, E, G, E, D, B, D, E, F, G};
    int noteDurations[] = {250, 250, 500, 500, 500, 250, 250, 500, 500, 250, 250, 250, 500, 500, 250, 500};

    for (int i = 0; i < 16; i++) {
        tone(BUZZ, melody[i]); 
        delay(noteDurations[i]); 
        noTone(BUZZ); 
        delay(50);
    }
}

