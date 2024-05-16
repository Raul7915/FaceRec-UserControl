import cv2
import sqlite3

faceDetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
cam = cv2.VideoCapture(0)


def insertorupdate(id, name):
    conn = sqlite3.connect("sqlite.db")
    cmd = "SELECT * FROM FACES WHERE ID=" + str(id)
    cursor = conn.execute(cmd)

    isRecordExist = 0
    for _ in cursor:
        isRecordExist = 1
    if isRecordExist == 1:
        conn.execute("UPDATE FACES SET Name=? WHERE ID=? ", (name, id,))
    else:
        conn.execute("INSERT INTO FACES (Id, Name) VALUES (?, ?)", (id, name,))

    conn.commit()
    conn.close()


Id = input('Enter User ID: ')
Name = input('Enter User Name: ')

insertorupdate(Id, Name)

sampleNum = 0
while True:
    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = faceDetect.detectMultiScale(gray, 1.3, 5)
    for (x, y, w, h) in faces:
        sampleNum = sampleNum + 1
        cv2.imwrite("dataset/user." + str(Id) + "." + str(sampleNum) + ".jpg", gray[y:y + h, x:x + w])
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.waitKey(100)
    cv2.imshow("Video", img)
    cv2.waitKey(1)
    if sampleNum > 200:
        break

cam.release()
cv2.destroyAllWindows()
