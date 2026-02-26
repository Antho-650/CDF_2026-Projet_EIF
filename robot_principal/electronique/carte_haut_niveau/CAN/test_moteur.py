import can
import time

# Configuration : on se branche sur 'can0'
bus = can.interface.Bus(channel='can0', bustype='socketcan')

def send_hex(id_moteur, command_string):
    # Nettoie la chaine (enlève les espaces) et convertit en octets
    data = bytearray.fromhex(command_string.replace(" ", ""))

    msg = can.Message(arbitration_id=id_moteur, data=data, is_extended_id=False)

    try:
        bus.send(msg)
        print(f"-> Envoyé à ID {id_moteur} : {command_string}")
    except can.CanError:
        print("!!! Erreur d'envoi !!!")

print("--- DÉMARRAGE DU TEST MOTEUR ---")

try:
    # 1. ACTIVER Moteur 1 (Enable)
    send_hex(1, "F3 01 F5")
    time.sleep(0.5)

    # 2. ACTIVER Moteur 2 (Enable)
    send_hex(2, "F3 01 F6")
    time.sleep(1)

    # 3. MOTEUR 1 : Tourne à vitesse moyenne (CW)
    print("Moteur 1 tourne...")
    send_hex(1, "F6 01 2C 05 29")

    # 4. MOTEUR 2 : Tourne à vitesse moyenne (CCW - Inverse)
    print("Moteur 2 tourne...")
    send_hex(2, "F6 81 2C 05 A9")

    # On laisse tourner 3 secondes
    time.sleep(3)

    # 5. ARRÊT DOUX (Stop)
    print("Arrêt des moteurs...")
    send_hex(1, "F6 00 00 50 47") # Arrêt ID 1
    send_hex(2, "F6 00 00 50 48") # Arrêt ID 2

    time.sleep(1)

    # 6. DÉSACTIVER (Roue Libre)
    print("Relâchement des moteurs...")
    send_hex(1, "F3 00 F4")
    send_hex(2, "F3 00 F5")

except KeyboardInterrupt:
    # Si on fait Ctrl+C, on arrête tout d'urgence
    send_hex(0, "F6 00 00 00 F7") # Arrêt d'urgence Broadcast

print("--- FIN DU TEST ---")
