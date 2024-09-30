import os
from PIL import Image

queue_dir = 'Queue'
combined_dir = 'Combined'

os.makedirs(combined_dir, exist_ok=True)

for texture_name in os.listdir(queue_dir):
    texture_path = os.path.join(queue_dir, texture_name)
    if os.path.isdir(texture_path):
        images = sorted([Image.open(os.path.join(texture_path, f)) for f in os.listdir(texture_path) if f.endswith('.png')])
        
        if images:
            widths, heights = zip(*(img.size for img in images))
            total_width = sum(widths)
            max_height = max(heights)
            
            combined_image = Image.new('RGBA', (total_width, max_height))
            x_offset = 0
            for img in images:
                combined_image.paste(img, (x_offset, 0))
                x_offset += img.width
            
            combined_image.save(os.path.join(combined_dir, f'{texture_name}.png'))