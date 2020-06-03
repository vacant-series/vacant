from imageai.Detection import ObjectDetection


isPerson  = False
detector = ObjectDetection()

model_path = "./models/yolo-tiny.h5"
input_path = "./input/t1.jpg"
output_path = "./output/newimage.jpg"

detector.setModelTypeAsTinyYOLOv3()
detector.setModelPath(model_path)
detector.loadModel(detection_speed="faster")
custom = detector.CustomObjects(person=True, dog=True)
detection = detector.detectCustomObjectsFromImage( custom_objects=custom,input_image=input_path,
 output_image_path=output_path, minimum_percentage_probability= 70)

for eachItem in detection:
    # print(eachItem["name"] , " : ", eachItem["percentage_probability"])
    if eachItem["name"] == "person":
        isPerson = True
        break
print("is person : ", isPerson)
