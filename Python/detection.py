import sqlite3
import cv2
from cvzone.SerialModule import SerialObject

arduino = SerialObject("COM7")

facedetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
cam = cv2.VideoCapture(0)

recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read("recognizer/trainingdata.yml")


def getprofile(id):
    conn = sqlite3.connect("sqlite.db")
    cursor = conn.execute("SELECT * FROM FACES WHERE id=?", (id,))
    profile = None
    for row in cursor:
        profile = row
    conn.close()
    return profile


clk = 0
recognised = False
while True:
    ret, img = cam.read()

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = facedetect.detectMultiScale(gray, 1.3, 5)

    for (x, y, w, h) in faces:
        id, conf = recognizer.predict(gray[y:y + h, x:x + w])
        if conf > 77:
            id = 0
        profile = getprofile(id)

        if profile is not None:
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

            cv2.putText(img, "Name: " + str(profile[1]), (x, y + h + 20), cv2.FONT_ITALIC, 1, (0, 255, 0), 2)
            arduino.sendData([1, 0, 0])
            print("ACCESS GRANTED!")
            clk = 0
        elif profile is None and clk < 150:
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 255), 2)
            cv2.putText(img, "Name:  unidentified", (x, y + h + 20), cv2.FONT_ITALIC, 1, (0, 255, 255), 2)
            arduino.sendData([0, 1, 0])
            print("ACCESS DENIED!")

            clk += 1
        else:
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.putText(img, "DANGER!", (x, y + h + 20), cv2.FONT_ITALIC, 1, (0, 0, 255), 2)
            arduino.sendData([0, 0, 1])
            print("ALERT!")

    cv2.imshow("Video", img)
    if cv2.waitKey(1) == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()
