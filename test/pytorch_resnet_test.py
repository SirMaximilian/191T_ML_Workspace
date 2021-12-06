import torch
import torchvision
from torchvision import transforms , datasets
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchsummary import summary # for model summary
import matplotlib.pyplot as plt # for ploting our data and showing images
import matplotlib.image as mpimg
import numpy as np
import cv2
import PIL.Image
#import dataHelper as dH
import os
import time
import random
#import ipywidgets
from IPython.display import display
#from utils import preprocess


device = torch.device('cuda')
output_dim = 2
# RESNET 18
model = torchvision.models.resnet18(pretrained=True)
model.fc = torch.nn.Linear(512, output_dim)
model = model.to(device)

summary(model, input_size=(3,224,224))