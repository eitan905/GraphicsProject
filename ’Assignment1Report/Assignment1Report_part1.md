#
Bresenham algorithm

```

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	int delta_x, delta_y, x_1, y_1, x_2, y_2, distance_to_line, doubled_delta_y,doubled_delta_x, doubled_delta_x_y, distance_to_end = 0,direction_x,direction_y,flag=0;

	x_1 = p1.x;
	y_1 = p1.y;
	x_2 = p2.x;
	y_2 = p2.y;

	//compute delta_x and delta_y
	delta_x = abs(x_2 - x_1);
	delta_y = abs(y_2 - y_1);

	//compute move_x,move_y
	if (x_1 < x_2) direction_x = 1;
	else direction_x = -1;

	if (y_1 < y_2) direction_y = 1;
	else direction_y = -1;

	if (delta_x == 0) {
		direction_x = 0;
	}
	
	//compute the step of the while

	if (delta_x < delta_y) {
		distance_to_end = delta_y;
		flag = 1;
	}
	else {
		distance_to_end = delta_x;
	}
	

	doubled_delta_y = 2 * delta_y;
	doubled_delta_x = 2 * delta_x;
	doubled_delta_x_y = doubled_delta_y - doubled_delta_x;
	//load (x_0,y_0) pixel
	PutPixel(x_1, y_1, color);

	//compute some const
	distance_to_line = 2 * delta_y - delta_x;
	while (distance_to_end > 0&&flag==0) {
		if (distance_to_line < 0 ) {
			x_1 += direction_x;
			distance_to_line = distance_to_line + doubled_delta_y;
		}
		else {
			x_1+= direction_x;
			y_1+= direction_y;
			distance_to_line = distance_to_line + doubled_delta_x_y;
		}
		PutPixel(x_1, y_1, color);
		distance_to_end--;
	}


	//compute some const
	distance_to_line = 2 * delta_x - delta_y;
	doubled_delta_x_y = doubled_delta_x - doubled_delta_y;
	while (distance_to_end > 0 && flag == 1) {
		if (distance_to_line < 0) {
			y_1 += direction_y;
			distance_to_line = distance_to_line + doubled_delta_x;
		} 
		else {
			y_1 += direction_y;
			x_1 += direction_x;
			distance_to_line = distance_to_line + doubled_delta_x_y;
		}
		PutPixel(x_1, y_1, glm::ivec3(1, 0, 1));
		distance_to_end--;
	}
}


```

