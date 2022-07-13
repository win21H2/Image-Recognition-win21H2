# Each time you see the below line, you have to make a new cell in Jupyter labs 
# Please note that it is reccommended to use the notebook provided by Nvidia as there are some config files which are not in this repository

#------------------------------------------------------------------------------------------------------------------
# Changes to the code made here
#------------------------------------------------------------------------------------------------------------------


#------------------------------------------------------------------------------------------------------------------
!pip3 install pyserial
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
!ls -ltrh /dev/video*
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
from jetcam.usb_camera import USBCamera
camera = USBCamera(width=500, height=500, capture_device=0)
camera.running = True
print("DONE")
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
import torchvision.transforms as transforms
from dataset import ImageClassificationDataset

TASK = 'colors'
CATEGORIES = ['blue', 'red']
DATASETS = ['A', 'B', 'C']

TRANSFORMS = transforms.Compose([
    transforms.ColorJitter(0.2, 0.2, 0.2, 0.2),
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

datasets = {}
for name in DATASETS:
    datasets[name] = ImageClassificationDataset('../data/classification/' + TASK + '_' + name, CATEGORIES, TRANSFORMS)  
    
print("{} task with {} categories defined".format(TASK, CATEGORIES))
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
DATA_DIR = '/nvdli-nano/data/classification/'
!mkdir -p {DATA_DIR}
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
import ipywidgets
import traitlets
from IPython.display import display
from jetcam.utils import bgr8_to_jpeg

dataset = datasets[DATASETS[0]]

camera.unobserve_all()

camera_widget = ipywidgets.Image()
traitlets.dlink((camera, 'value'), (camera_widget, 'value'), transform=bgr8_to_jpeg)

dataset_widget = ipywidgets.Dropdown(options=DATASETS, description='dataset')
category_widget = ipywidgets.Dropdown(options=dataset.categories, description='category')
count_widget = ipywidgets.IntText(description='count')
save_widget = ipywidgets.Button(description='add')

count_widget.value = dataset.get_count(category_widget.value)

def set_dataset(change):
    global dataset
    dataset = datasets[change['new']]
    count_widget.value = dataset.get_count(category_widget.value)
dataset_widget.observe(set_dataset, names='value')

def update_counts(change):
    count_widget.value = dataset.get_count(change['new'])
category_widget.observe(update_counts, names='value')

def save(c):
    dataset.save_entry(camera.value, category_widget.value)
    count_widget.value = dataset.get_count(category_widget.value)
save_widget.on_click(save)

data_collection_widget = ipywidgets.VBox([
    ipywidgets.HBox([camera_widget]), dataset_widget, category_widget, count_widget, save_widget
])

print("DONE")
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
import torch
import torchvision

device = torch.device('cuda')

# RESNET 18 MODEL
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, len(dataset.categories))
    
model = model.to(device)

model_save_button = ipywidgets.Button(description='save model')
model_load_button = ipywidgets.Button(description='load model')
model_path_widget = ipywidgets.Text(description='model path', value='/nvdli-nano/data/classification/my_model.pth')

def load_model(c):
    model.load_state_dict(torch.load(model_path_widget.value))
model_load_button.on_click(load_model)
    
def save_model(c):
    torch.save(model.state_dict(), model_path_widget.value)
model_save_button.on_click(save_model)

model_widget = ipywidgets.VBox([
    model_path_widget,
    ipywidgets.HBox([model_load_button, model_save_button])
])

print("DONE")
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
import threading
import time
from utils import preprocess
import torch.nn.functional as F
import serial
import time

prediction_string = ""
prediction_score = 0
state_widget = ipywidgets.ToggleButtons(options=['stop', 'live'], description='state', value='stop')
prediction_widget = ipywidgets.Text(description='prediction')
output_widget = ipywidgets.Button(description='getvalue')
score_widgets = []
for category in dataset.categories:
    score_widget = ipywidgets.FloatSlider(min=0.0, max=10.0, description=category, orientation='vertical')
    score_widgets.append(score_widget)

def live(state_widget, model, camera, prediction_widget, score_widget):
    global dataset
    global prediction_string
    global prediction_score
    ser = serial.Serial('/dev/ttyACM0', 11520, timeout=10)
    while state_widget.value == 'live':
        image = camera.value
        preprocessed = preprocess(image)
        output = model(preprocessed)
        output = F.softmax(output, dim=1).detach().cpu().numpy().flatten()
        category_index = output.argmax()
        prediction_widget.value = dataset.categories[category_index]
        prediction_string = dataset.categories[category_index]
        prediction_score = list(output)[category_index]

        if prediction_score > 0.7 and prediction_string == 'red':
            time.sleep(0.1)
            ser.write(bytes('RED\n','utf-8'))
        if prediction_score > 0.7 and prediction_string == 'blue':
            time.sleep(0.1)
            ser.write(bytes('BLUE\n','utf-8'))
        if prediction_score > 0.7 and prediction_string == 'off':
            time.sleep(0.1)
            ser.write(bytes('OFF\n','utf-8'))
            
        for i, score in enumerate(list(output)):
            score_widgets[i].value = score    
            
def start_live(change):
    if change['new'] == 'live':
        execute_thread = threading.Thread(target=live, args=(state_widget, model, camera, prediction_widget, score_widget))
        execute_thread.start()                  
                     
state_widget.observe(start_live, names='value')
live_execution_widget = ipywidgets.VBox([
    ipywidgets.HBox(score_widgets),
    prediction_widget,
    state_widget,
    output_widget
])

print("DONE")
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
BATCH_SIZE = 8

optimizer = torch.optim.Adam(model.parameters())

epochs_widget = ipywidgets.IntText(description='epochs', value=1)
eval_button = ipywidgets.Button(description='evaluate')
train_button = ipywidgets.Button(description='train')
loss_widget = ipywidgets.FloatText(description='loss')
accuracy_widget = ipywidgets.FloatText(description='accuracy')
progress_widget = ipywidgets.FloatProgress(min=0.0, max=1.0, description='progress')

def train_eval(is_training):
    global BATCH_SIZE, LEARNING_RATE, MOMENTUM, model, dataset, optimizer, eval_button, train_button, accuracy_widget, loss_widget, progress_widget, state_widget
    
    try:
        train_loader = torch.utils.data.DataLoader(
            dataset,
            batch_size=BATCH_SIZE,
            shuffle=True
        )

        state_widget.value = 'stop'
        train_button.disabled = True
        eval_button.disabled = True
        time.sleep(1)

        if is_training:
            model = model.train()
        else:
            model = model.eval()
        while epochs_widget.value > 0:
            i = 0
            sum_loss = 0.0
            error_count = 0.0
            for images, labels in iter(train_loader):
                # send data to device
                images = images.to(device)
                labels = labels.to(device)

                if is_training:
                    # zero gradients of parameters
                    optimizer.zero_grad()

                # execute model to get outputs
                outputs = model(images)

                # compute loss
                loss = F.cross_entropy(outputs, labels)

                if is_training:
                    # run backpropogation to accumulate gradients
                    loss.backward()

                    # step optimizer to adjust parameters
                    optimizer.step()

                # increment progress
                error_count += len(torch.nonzero(outputs.argmax(1) - labels).flatten())
                count = len(labels.flatten())
                i += count
                sum_loss += float(loss)
                progress_widget.value = i / len(dataset)
                loss_widget.value = sum_loss / i
                accuracy_widget.value = 1.0 - error_count / i
                
            if is_training:
                epochs_widget.value = epochs_widget.value - 1
            else:
                break
    except e:
        pass
    model = model.eval()

    train_button.disabled = False
    eval_button.disabled = False
    state_widget.value = 'live'
    
train_button.on_click(lambda c: train_eval(is_training=True))
eval_button.on_click(lambda c: train_eval(is_training=False))
  
train_eval_widget = ipywidgets.VBox([
    epochs_widget,
    progress_widget,
    loss_widget,
    accuracy_widget,
    ipywidgets.HBox([train_button, eval_button])
])

print("DONE")
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
all_widget = ipywidgets.VBox([
    ipywidgets.HBox([data_collection_widget, live_execution_widget]), 
    train_eval_widget,
    model_widget
])

display(all_widget)
#------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------
# Shut down the camera
import os
import IPython
os._exit(00)
#------------------------------------------------------------------------------------------------------------------