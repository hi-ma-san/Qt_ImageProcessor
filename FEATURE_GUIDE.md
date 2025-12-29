# Image Region Capture and Editor - Feature Guide

## Overview
This document describes the newly implemented image region capture and editing features.

## How to Use

### 1. Region Selection
1. Open an image file using the "開啟檔案" (Open File) menu or toolbar button
2. When an image is displayed in the main window, click and drag on the image to select a region
3. A rubber band selection rectangle will appear as you drag
4. Release the mouse button to capture the selected region
5. The captured region will open in a new Image Editor window

### 2. Image Editor Features

The Image Editor window provides the following functionality:

#### Drawing Pen (畫筆)
- Click and drag on the image to draw with the pen
- Default pen color is red
- All drawings are preserved and can be saved with the image

#### Change Color (變更顏色)
- Click the "變更顏色" button to open a color picker dialog
- Select any color for the drawing pen
- The new color will be applied to subsequent drawings

#### Rotate Left (向左旋轉)
- Click the "向左旋轉" button to rotate the image 90 degrees counter-clockwise
- Note: Drawings will be cleared when rotating to maintain consistency

#### Rotate Right (向右旋轉)
- Click the "向右旋轉" button to rotate the image 90 degrees clockwise
- Note: Drawings will be cleared when rotating to maintain consistency

#### Save (存檔)
- Click the "存檔" button to save the modified image
- A file dialog will appear to select the save location and format
- Supported formats: PNG, JPEG, BMP
- The saved image will include all drawings made in the editor

## Technical Implementation

### Key Components

1. **ImageEditor Class (ie.h/ie.cpp)**
   - Manages the image editing window
   - Handles drawing operations with mouse events
   - Provides UI controls for color change, rotation, and saving

2. **IP Class Updates (ip.h/ip.cpp)**
   - Added QRubberBand for visual region selection feedback
   - Enhanced mouse event handlers to track selection
   - Extracts selected region and launches ImageEditor

3. **Drawing System**
   - Stores drawing strokes as vectors of points
   - Each stroke has an associated color
   - Strokes are rendered on top of the image in real-time
   - Final save operation merges all strokes into the image

## Code Quality
- All code follows Qt naming conventions
- Proper memory management with Qt parent-child relationships
- Use of Qt signals and slots for event handling
- Deprecation warnings addressed (pixmap() method)
