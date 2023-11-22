#include "Tree.hpp"
#include "Maths.hpp"

Model Plant::getBlackberryBushModel(float x, float y, float z)
{
	Model model;

	TextureFace leftTop(2.0f, 3);
	TextureFace leftBottom(2.0f, 2.0f);

	TextureFace middleTop(3, 3);
	TextureFace middleBottom(3, 2.0f);

	TextureFace rightTop(4, 3);
	TextureFace rightBottom(4, 2);

	TextureFace bushTop(0, 4);

	int width = 4 * rando() + (rando() * 2.0f);
	int depth = 4 * rando() + (rando() * 2.0f);

	glm::vec3 frontLeft(x, y, z);
	glm::vec3 backLeft(x, y, z+depth+2.0f);

	glm::vec3 frontRight(x+width+2.0f, y, z);
	glm::vec3 backRight(x+width+2.0f, y, z + depth + 2.0f);

	glm::vec3 leanBack(0, 1, -0.5);
	glm::vec3 leanForward(0, 1, 0.5);
	glm::vec3 leanLeft(-0.5, 1, 0);
	glm::vec3 leanRight(0.5, 1, 0);

	//LEFT SIDE CORNERS 
	model.verts.insert(model.verts.end(), {
		    //front left, lean forward corner
			frontLeft.x,y,frontLeft.z,
			frontLeft.x + 1,y,frontLeft.z,
			frontLeft.x + 1 + leanForward.x * 2.0f,y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,

			frontLeft.x + 1 + leanForward.x * 2.0f,y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,
			frontLeft.x + leanForward.x * 2.0f,y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,
			frontLeft.x,frontLeft.y,frontLeft.z,

			//front left, lean right corner
			frontLeft.x ,y,frontLeft.z+ 1,
			frontLeft.x,y,frontLeft.z,
			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + leanRight.z * 2.0f,

			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + leanRight.z * 2.0f,
			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + 1 + leanRight.z * 2.0f,
			frontLeft.x ,y,frontLeft.z + 1,

			//back left, lean back corner
			backLeft.x + 1, y, backLeft.z,
			backLeft.x, y, backLeft.z,
			backLeft.x + leanBack.x*2.0f, y +leanBack.y*2.0f, backLeft.z+leanBack.z*2.0f,

			backLeft.x + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backLeft.z + leanBack.z * 2.0f,
			backLeft.x + 1 + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backLeft.z + leanBack.z * 2.0f,
			backLeft.x + 1, y, backLeft.z,

			//back left, lean right corner
			backLeft.x, y, backLeft.z,
			backLeft.x, y, backLeft.z - 1,
			backLeft.x + leanRight.x*2.0f, y + leanRight.y*2.0f, backLeft.z - 1 + leanRight.z*2.0f,

			backLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f, backLeft.z - 1 + leanRight.z * 2.0f,
			backLeft.x + leanRight.x * 2.0f, y + leanRight.y * 2.0f, backLeft.z + leanRight.z * 2.0f,
			backLeft.x, y, backLeft.z,

			//front right, lean forward corner
			frontRight.x-1, y, frontRight.z,
			frontRight.x, y, frontRight.z,
			frontRight.x + leanForward.x*2.0f, y + leanForward.y * 2.0f, frontRight.z + leanForward.z * 2.0f,

			frontRight.x + leanForward.x * 2.0f, y + leanForward.y * 2.0f, frontRight.z + leanForward.z * 2.0f,
			frontRight.x - 1 + leanForward.x * 2.0f, y + leanForward.y * 2.0f, frontRight.z + leanForward.z * 2.0f,
			frontRight.x - 1, y, frontRight.z,

			//front right, lean left corner
			frontRight.x, y, frontRight.z,
			frontRight.x, y, frontRight.z+1,
			frontRight.x + leanLeft.x*2.0f, y + leanLeft.y * 2.0f, frontRight.z + 1 + leanLeft.z * 2.0f,

			frontRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, frontRight.z + 1 + leanLeft.z * 2.0f,
			frontRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, frontRight.z + leanLeft.z * 2.0f,
			frontRight.x, y, frontRight.z,

			//back right, lean back corner
			backRight.x - 1, y, backRight.z,
			backRight.x, y, backRight.z,
			backRight.x + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backRight.z + leanBack.z * 2.0f,

			backRight.x + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backRight.z + leanBack.z * 2.0f,
			backRight.x -1 + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backRight.z + leanBack.z * 2.0f,
			backRight.x - 1, y, backRight.z,

			//back right, lean left corner
			backRight.x, y, backRight.z-1,
			backRight.x, y, backRight.z,
			backRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, backRight.z + leanLeft.z * 2.0f,

			backRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, backRight.z + leanLeft.z * 2.0f,
			backRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f , backRight.z -1 + leanLeft.z * 2.0f,
			backRight.x, y, backRight.z - 1,
		});
	model.uvs.insert(model.uvs.end(), {
			//front left lean forward
			leftBottom.tl.x, leftBottom.tl.y,
			leftBottom.tr.x, leftBottom.tr.y,
			leftTop.br.x, leftTop.br.y,

			leftTop.br.x, leftTop.br.y,
			leftTop.bl.x, leftTop.bl.y,
			leftBottom.tl.x, leftBottom.tl.y,
			//front left lean right
			rightBottom.tl.x, rightBottom.tl.y,
			rightBottom.tr.x, rightBottom.tr.y,
			rightTop.br.x, rightTop.br.y,

			rightTop.br.x, rightTop.br.y,
			rightTop.bl.x, rightTop.bl.y,
			rightBottom.tl.x, rightBottom.tl.y,
			//back left lean back
			rightBottom.tl.x, rightBottom.tl.y,
			rightBottom.tr.x, rightBottom.tr.y,
			rightTop.br.x, rightTop.br.y,

			rightTop.br.x, rightTop.br.y,
			rightTop.bl.x, rightTop.bl.y,
			rightBottom.tl.x, rightBottom.tl.y,
			//back left lean right
			leftBottom.tl.x, leftBottom.tl.y,
			leftBottom.tr.x, leftBottom.tr.y,
			leftTop.br.x, leftTop.br.y,

			leftTop.br.x, leftTop.br.y,
			leftTop.bl.x, leftTop.bl.y,
			leftBottom.tl.x, leftBottom.tl.y,
			//front right lean forward
			rightBottom.tl.x, rightBottom.tl.y,
			rightBottom.tr.x, rightBottom.tr.y,
			rightTop.br.x, rightTop.br.y,

			rightTop.br.x, rightTop.br.y,
			rightTop.bl.x, rightTop.bl.y,
			rightBottom.tl.x, rightBottom.tl.y,
			//front right lean left
			leftBottom.tl.x, leftBottom.tl.y,
			leftBottom.tr.x, leftBottom.tr.y,
			leftTop.br.x, leftTop.br.y,

			leftTop.br.x, leftTop.br.y,
			leftTop.bl.x, leftTop.bl.y,
			leftBottom.tl.x, leftBottom.tl.y,
			//back right lean back
			rightBottom.tl.x, rightBottom.tl.y,
			rightBottom.tr.x, rightBottom.tr.y,
			rightTop.br.x, rightTop.br.y,

			rightTop.br.x, rightTop.br.y,
			rightTop.bl.x, rightTop.bl.y,
			rightBottom.tl.x, rightBottom.tl.y,
			//back right lean left
			rightBottom.tl.x, rightBottom.tl.y,
			rightBottom.tr.x, rightBottom.tr.y,
			rightTop.br.x, rightTop.br.y,

			rightTop.br.x, rightTop.br.y,
			rightTop.bl.x, rightTop.bl.y,
			rightBottom.tl.x, rightBottom.tl.y,
		});

	model.cols.insert(model.cols.end(), {
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
		});
	//WIDTH MIDDLES (FRONT-LEAN-FORWARD and BACK-LEAN-BACK)
	for (int i = 1; i < width + 1; i++)
	{
		model.verts.insert(model.verts.end(), {
			//front left, lean forward corner
			frontLeft.x + i, y,frontLeft.z,
			frontLeft.x + i + 1,y,frontLeft.z,
			frontLeft.x + i + 1 + leanForward.x * 2.0f,y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,

			frontLeft.x + i + 1 + leanForward.x * 2.0f, y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,
			frontLeft.x + i + leanForward.x * 2.0f,y + leanForward.y * 2.0f,frontLeft.z + leanForward.z * 2.0f,
			frontLeft.x + i,y,frontLeft.z,

			//back left, lean back corner
			backLeft.x + i + 1, y, backLeft.z,
			backLeft.x + i , y, backLeft.z,
			backLeft.x + i + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backLeft.z + leanBack.z * 2.0f,

			backLeft.x + i + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backLeft.z + leanBack.z * 2.0f,
			backLeft.x + i + 1 + leanBack.x * 2.0f, y + leanBack.y * 2.0f, backLeft.z + leanBack.z * 2.0f,
			backLeft.x + i + 1, y, backLeft.z,
			});
		model.uvs.insert(model.uvs.end(), {
			middleBottom.tl.x, middleBottom.tl.y,
			middleBottom.tr.x, middleBottom.tr.y,
			middleTop.br.x, middleTop.br.y,

			middleTop.br.x, middleTop.br.y,
			middleTop.bl.x, middleTop.bl.y,
			middleBottom.tl.x, middleBottom.tl.y,

			middleBottom.tl.x, middleBottom.tl.y,
			middleBottom.tr.x, middleBottom.tr.y,
			middleTop.br.x, middleTop.br.y,

			middleTop.br.x, middleTop.br.y,
			middleTop.bl.x, middleTop.bl.y,
			middleBottom.tl.x, middleBottom.tl.y,
			});
		model.cols.insert(model.cols.end(), {
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			});
	}
	//DEPTH MIDDLES (ONLY ON LEFT SIDE) ADDING RIGHT RN
	for (int i = 1; i < depth + 1; i++)
	{
		model.verts.insert(model.verts.end(), {
			//front left, lean right corner
			frontLeft.x ,y,frontLeft.z + 1 + i,
			frontLeft.x,y,frontLeft.z + i,
			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + i + leanRight.z * 2.0f,

			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + i + leanRight.z * 2.0f,
			frontLeft.x + leanRight.x * 2.0f,y + leanRight.y * 2.0f,frontLeft.z + i + 1 + leanRight.z * 2.0f,
			frontLeft.x ,y,frontLeft.z + i + 1,

			//front right, lean left corner
			frontRight.x, y, frontRight.z + i,
			frontRight.x, y, frontRight.z + i + 1,
			frontRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, frontRight.z + i + 1 + leanLeft.z * 2.0f,

			frontRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, frontRight.z + i + 1 + leanLeft.z * 2.0f,
			frontRight.x + leanLeft.x * 2.0f, y + leanLeft.y * 2.0f, frontRight.z + i + leanLeft.z * 2.0f,
			frontRight.x, y, frontRight.z + i,
			});
		model.uvs.insert(model.uvs.end(), {
			middleBottom.tl.x, middleBottom.tl.y,
			middleBottom.tr.x, middleBottom.tr.y,
			middleTop.br.x, middleTop.br.y,

			middleTop.br.x, middleTop.br.y,
			middleTop.bl.x, middleTop.bl.y,
			middleBottom.tl.x, middleBottom.tl.y,

			middleBottom.tl.x, middleBottom.tl.y,
			middleBottom.tr.x, middleBottom.tr.y,
			middleTop.br.x, middleTop.br.y,

			middleTop.br.x, middleTop.br.y,
			middleTop.bl.x, middleTop.bl.y,
			middleBottom.tl.x, middleBottom.tl.y,

			});
		model.cols.insert(model.cols.end(), {
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,

			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			});
	}

	for (float i = 0.5; i < width + 1 + 0.5; i++)
	{
		for (float d = 0.5; d < depth + 1 + 0.5; d++)
		{
			model.verts.insert(model.verts.end(), {
					frontLeft.x + i, y + 1.1f, frontLeft.z + d,
					frontLeft.x + i + 1, y + 1.1f, frontLeft.z + d,
					frontLeft.x + i + 1,y + 1.1f, frontLeft.z + d + 1,

					frontLeft.x + i + 1, y + 1.1f, frontLeft.z + d + 1,
					frontLeft.x + i, y + 1.1f, frontLeft.z + d + 1,
					frontLeft.x + i, y + 1.1f, frontLeft.z + d,
				});
			model.uvs.insert(model.uvs.end(), {
					bushTop.tl.x, bushTop.tl.y,
					bushTop.tr.x, bushTop.tr.y,
					bushTop.br.x, bushTop.br.y,

					bushTop.br.x, bushTop.br.y,
					bushTop.bl.x, bushTop.bl.y,
					bushTop.tl.x, bushTop.tl.y,
				});
			model.cols.insert(model.cols.end(), {
					1.0, 1.0, 1.0,
					1.0, 1.0, 1.0,
					1.0, 1.0, 1.0,

					1.0, 1.0, 1.0,
					1.0, 1.0, 1.0,
					1.0, 1.0, 1.0,
				});
		}
	}

	return model;
}