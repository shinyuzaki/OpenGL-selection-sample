#include "stdafx.h"


// �N���b�v�{�b�N�X
struct clip_box
{
	// �s�b�N�n���h���C���f�b�N�X
	enum handle {
		handle_none = 0,	//!< �s�b�N�Ȃ�
		handle_box_x_pos,	//!< box x+
		handle_box_x_neg,	//!< box x-
		handle_box_y_pos,	//!< box y+
		handle_box_y_neg,	//!< box y-
		handle_box_z_pos,	//!< box z+
		handle_box_z_neg,	//!< box z-
		handle_trans_x,
		handle_trans_y,
		handle_trans_z,
		handle_trans_xyz,
		handle_rot_x,
		handle_rot_y,
		handle_rot_z,
		handle_cnt,
	};
	// ���W���C���f�b�N�X
	enum axis {
		axis_x_pos = 0,
		axis_x_neg,
		axis_y_pos,
		axis_y_neg,
		axis_z_pos,
		axis_z_neg,
		axis_cnt,
	};

	// �`��F�ݒ�.
	void setup_color(
		handle condition	//!< [in] m_active_handle�Ɠ������ꍇ�n�C���C�g�F
		) const
	{
		if (m_active_handle == condition)
		{
			glColor4fv(m_handle_highlight_color);
		}
		else
		{
			glColor4fv(m_handle_color);
		}
	}
	
	// ����`�悷��.
	void draw_cube(
		const float* box	//!< [in] enum axis �ŎQ�Ƃ���z��.
		) const
	{
		glBegin(GL_QUADS);
		// +x
		glNormal3f(1, 0, 0);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_neg]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_neg]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_pos]);
		// -x
		glNormal3f(-1, 0, 0);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_neg]);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_pos]);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_neg]);
		// +y
		glNormal3f(0, 1, 0);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_pos]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_pos]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_neg]);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_neg]);
		// -y
		glNormal3f(0, -1, 0);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_neg]);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_neg]);
		// +z
		glNormal3f(0, 0, 1);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_pos]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_pos]);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_pos]);
		// -z
		glNormal3f(0, 0, -1);
		glVertex3f(box[axis_x_pos], box[axis_y_neg], box[axis_z_neg]);
		glVertex3f(box[axis_x_neg], box[axis_y_neg], box[axis_z_neg]);
		glVertex3f(box[axis_x_neg], box[axis_y_pos], box[axis_z_neg]);
		glVertex3f(box[axis_x_pos], box[axis_y_pos], box[axis_z_neg]);
		glEnd();
	}
	// �}�j�s�����[�^�n���h���`��.
	// �`��R�[�h����Name stack���d����ł܂�.
	void draw_handle() const
	{
		// �}�j�s�����[�^�n���h����`�悷��ۂ�depth test��؂�
		glDisable(GL_DEPTH_TEST);

		/////////////////////////////////
		// trans
		glBlendFunc(GL_ONE, GL_ZERO);

		// x
		setup_color(handle_trans_x);
		glPushName(handle_trans_x);
		glBegin(GL_LINES);
		glVertex3f( m_handle_axis_length, 0, 0);
		glVertex3f(-m_handle_axis_length, 0, 0);
		glEnd();
		glPopName();

		// y
		setup_color(handle_trans_y);
		glPushName(handle_trans_y);
		glBegin(GL_LINES);
		glVertex3f(0,  m_handle_axis_length, 0);
		glVertex3f(0, -m_handle_axis_length, 0);
		glEnd();
		glPopName();

		// z
		setup_color(handle_trans_z);
		glPushName(handle_trans_z);
		glBegin(GL_LINES);
		glVertex3f(0, 0, m_handle_axis_length);
		glVertex3f(0, 0, -m_handle_axis_length);
		glEnd();
		glPopName();

		// xyz
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushName(handle_trans_xyz);

		float trans_box[] = {
			m_handle_cube_size,
			-m_handle_cube_size,
			m_handle_cube_size,
			-m_handle_cube_size,
			m_handle_cube_size,
			-m_handle_cube_size,
		};
		setup_color(handle_trans_xyz);
		draw_cube(trans_box);

		glPopName();
		glBlendFunc(GL_ONE, GL_ZERO);

		/////////////////////////////////
		// box
		glBlendFunc(GL_ONE, GL_ZERO);

		// x+
		setup_color(handle_box_x_pos);
		glPushName(handle_box_x_pos);
		glBegin(GL_LINES);
		glVertex3f(m_box[axis_x_pos], 0, 0);
		glVertex3f(m_box[axis_x_pos] + m_handle_axis_length, 0, 0);
		glEnd();
		glPopName();

		// x-
		setup_color(handle_box_x_neg);
		glPushName(handle_box_x_neg);
		glBegin(GL_LINES);
		glVertex3f(m_box[axis_x_neg], 0, 0);
		glVertex3f(m_box[axis_x_neg] - m_handle_axis_length, 0, 0);
		glEnd();
		glPopName();

		// y+
		setup_color(handle_box_y_pos);
		glPushName(handle_box_y_pos);
		glBegin(GL_LINES);
		glVertex3f(0, m_box[axis_y_pos], 0);
		glVertex3f(0, m_box[axis_y_pos] + m_handle_axis_length, 0);
		glEnd();
		glPopName();

		// y-
		setup_color(handle_box_y_neg);
		glPushName(handle_box_y_neg);
		glBegin(GL_LINES);
		glVertex3f(0, m_box[axis_y_neg], 0);
		glVertex3f(0, m_box[axis_y_neg] - m_handle_axis_length, 0);
		glEnd();
		glPopName();

		// z+
		setup_color(handle_box_z_pos);
		glPushName(handle_box_z_pos);
		glBegin(GL_LINES);
		glVertex3f(0, 0, m_box[axis_z_pos]);
		glVertex3f(0, 0, m_box[axis_z_pos] + m_handle_axis_length);
		glEnd();
		glPopName();

		// z-
		setup_color(handle_box_z_neg);
		glPushName(handle_box_z_neg);
		glBegin(GL_LINES);
		glVertex3f(0, 0, m_box[axis_z_neg]);
		glVertex3f(0, 0, m_box[axis_z_neg] - m_handle_axis_length);
		glEnd();
		glPopName();

		/////////////////////////////////
		glEnable(GL_DEPTH_TEST);
	}

	// �N���b�v�{�b�N�X�ƃ}�j�s�����[�^�n���h���`��
	void draw(
		bool use_handle	//!< [in] true�̏ꍇ�}�j�s�����[�^�n���h���`��
		) const
	{
		glPushMatrix();

#if 0
		glTranslatef(m_trans.x, m_trans.y, m_trans.z);
		glRotatef(m_rot.x, 1, 0, 0);
		glRotatef(m_rot.y, 0, 1, 0);
		glRotatef(m_rot.z, 0, 0, 1);
#else
		glTranslatef(m_trans.x, m_trans.y, m_trans.z);
		glRotatef(m_rot.z, 0, 0, 1);
		glRotatef(m_rot.y, 0, 1, 0);
		glRotatef(m_rot.x, 1, 0, 0);
#endif
		glColor4fv(m_cube_color);

		glBlendFunc(GL_ONE, GL_ZERO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_cube(m_box);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		draw_cube(m_box);

		glBlendFunc(GL_ONE, GL_ZERO);

		if (use_handle) 
		{
			draw_handle();
		}

		glPopMatrix();
	}

	// �s�b�N����
	// @return �q�b�g�����}�j�s�����[�^
	handle hit
		(int x,			//!< [in] �}�E�X���WX
		 int y,			//!< [in] �}�E�X���WY
		 int area,		//!< [in] �s�b�N�͈�(�E�B���h�E���W�n)
		 double& depth	//!< [out] �s�b�N�ʒu�f�v�X
		 ) const
	{
		depth = 0;

		// ���݂̃r���[�|�[�g/���e�s��𓾂�
		GLint viewport[4];
		CG::mat4d projection_matrix;
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

		// PickMatrix�Ɍ��݂̓��e�s�����Z����
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();		// �X�^�b�N����i�グ�Č��݂̏�Ԃ�ޔ�
		glLoadIdentity();
		// PickMatrix
		gluPickMatrix(x, viewport[3] - y, area, area, viewport);
		// ���݂̓��e�s��
		glMultMatrixd(projection_matrix);
		glMatrixMode(GL_MODELVIEW);

		// selection draw�̏���
		GLuint pick_buffer[64];
		glSelectBuffer(sizeof(pick_buffer)/sizeof(*pick_buffer), pick_buffer);
		glRenderMode(GL_SELECT);
		glInitNames();

		// �`��͒ʏ�ʂ�
		draw(true);

		// �ޔ��������e�s��𕜋A
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		// �ʏ�`��ɖ߂��ƃq�b�g�����Ԃ��Ă���
		int hits = glRenderMode(GL_RENDER);

		// selection ���ʉ��
		handle hit_handle = handle_none;

		if (hits > 0)
		{
			GLuint* p = pick_buffer;

			GLuint name_depth = 0xffffffff;
			for (; hits > 0; hits--)
			{
				GLuint names = *p++;
				GLuint depth = *p++; // Z near
				p++; // Z far

				// ��Ԏ�O�̑I���𓾂�
				if (names > 0 &&
					depth < name_depth)
				{
					name_depth = depth;
					hit_handle = static_cast<handle>(p[0]);
				}
				p += names;
			}

			depth = double(name_depth) / ULONG_MAX;
		}
		return hit_handle;
	}

	// �R���X�g���N�^
	clip_box()
		: m_trans(0.0f)
		, m_rot(0.0f)
		, m_active_handle(handle_none)
		, m_cube_color(0.5f, 0.1f, 0.5f, 0.5f)
		, m_handle_color(0.5f, 0.5f, 0.5f, 0.5f)
		, m_handle_highlight_color(0.8f, 0.8f, 0.1f, 0.5f)
		, m_handle_cube_size(0.25f)
		, m_handle_axis_length(0.75f)
	{
		m_box[axis_x_pos] = 1;
		m_box[axis_x_neg] = -1;
		m_box[axis_y_pos] = 1;
		m_box[axis_y_neg] = -1;
		m_box[axis_z_pos] = 1;
		m_box[axis_z_neg] = -1;
	}

	float m_handle_cube_size;			//!< translate �L���[�u�n���h���̃T�C�Y
	float m_handle_axis_length;			//!< �����ƃn���h���̒���
	CG::vec4f m_cube_color;				//!< �N���b�v�{�b�N�X�̐F
	CG::vec4f m_handle_color;			//!< �}�j�s�����[�^�n���h���̒ʏ�F
	CG::vec4f m_handle_highlight_color;	//!< �}�j�s�����[�^�n���h���̃n�C���C�g�F
	handle	  m_active_handle;			//!< ���݃A�N�e�B�u�ȃ}�j�s�����[�^�n���h��

	CG::vec3f m_trans;					//!< �N���b�v�{�b�N�X�ʒu
	CG::vec3f m_rot;					//!< �N���b�v�{�b�N�X�I�C���[�p(degree)
	float	  m_box[6];					//!< �N���b�v�{�b�N�X�ʒu����N���b�v�ʂւ̋���
};

// �}�E�X�J�[�\��
struct cursor
{
	// 2D����3D�փ}�E�X���W��ϊ�����
	// @return 3D�}�E�X���W
	CG::vec3f unproject
		(int x,			//!< [in] �}�E�X���WX
		 int y,			//!< [in] �}�E�X���WY
		 double depth	//!< [in] �s�b�N�ʒu�f�v�X
		 ) const
	{
		// ���݂̃r���[�|�[�g/���e�s��𓾂�
		GLint viewport[4];
		CG::mat4d projection_matrix;
		CG::mat4d modelview_matrix;

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);

		// �������g�p����2D�}�E�X���W��3D�ɕϊ�����
		CG::vec3d pos;
		gluUnProject(x, viewport[3] - y, depth,
			modelview_matrix, projection_matrix, viewport,
			&pos.x, &pos.y, &pos.z);

		return CG::vec3f(pos.x, pos.y, pos.z);
	}
	cursor()
		: m_in_drag(false)
	{}

	bool m_in_drag;
	double m_depth;
	CG::vec2<int> m_2d;
	CG::vec3f m_3d;
};

///////////////////////////////////////
clip_box g_box;
cursor g_cursor;
CG::vec3f g_eye(0.0f, 0.0f, 10.0f);

///////////////////////////////////////
void update_camera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		g_eye.x, g_eye.y, g_eye.z,
		0, 0, 0,
		//g_box.m_trans.x, g_box.m_trans.y, g_box.m_trans.z,
		0, 1, 0);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	update_camera();
	g_box.draw(true);
	glutSwapBuffers();
}
void mouse(int button, int state, int x, int y)
{
	if (g_cursor.m_in_drag = (state == GLUT_DOWN))
	{
		// �}�E�X�����ʒu���o���Ă���
		g_cursor.m_2d.x = x;
		g_cursor.m_2d.y = y;
		g_cursor.m_3d = g_cursor.unproject(x, y, g_cursor.m_depth);
	}
}
void passive_motion(int x, int y)
{
	// �����}�E�X�������Ă鎞�Ƀ}�j�s�����[�^�s�b�N���s�Ȃ�
	update_camera();
	clip_box::handle h = g_box.hit(x, y, 4, g_cursor.m_depth);

	fprintf(stderr, "hit: %d / depth:%lf\n", int(h), g_cursor.m_depth);

	if (g_box.m_active_handle != h)
	{
		g_box.m_active_handle = h;
		glutPostRedisplay();
	}
}
void motion(int x, int y)
{
	// �}�E�X���݈ʒu
	CG::vec2<int> c2d(x, y);

	// passive_motion()�Ń}�j�s�����[�^�Ƀ}�E�X���W���q�b�g���Ă鎞�����������s��
	if (g_box.m_active_handle)
	{
		// �}�E�X���݈ʒu
		CG::vec3f c3d = g_cursor.unproject(x, y, g_cursor.m_depth);

		fprintf(stderr, "c3d:(%f,%f,%f)\n", c3d.x, c3d.y, c3d.z); 

		switch (g_box.m_active_handle)
		{
		case clip_box::handle_box_x_pos:	//!< box x+
		case clip_box::handle_box_x_neg:	//!< box x-
		case clip_box::handle_box_y_pos:	//!< box y+
		case clip_box::handle_box_y_neg:	//!< box y-
		case clip_box::handle_box_z_pos:	//!< box z+
		case clip_box::handle_box_z_neg:	//!< box z-
			{
				int i =
					g_box.m_active_handle - clip_box::handle_box_x_pos;
				int axis = i / 2;
				g_box.m_box[i] += (c3d - g_cursor.m_3d)[axis];
			}
			break;
		case clip_box::handle_trans_x:
		case clip_box::handle_trans_y:
		case clip_box::handle_trans_z:
			{
				int axis = g_box.m_active_handle - clip_box::handle_trans_x;
#if 0	// ���������ƈړ�
				g_box.m_trans[axis] += (c3d - g_cursor.m_3d)[axis];
#else	// ���������Ɖ�](�{���̓L�[���f�B�t�@�C�A(Ctrl key)�Ƃ��Ő؂�ւ�
				g_box.m_rot[axis] += CG::degrees((c3d - g_cursor.m_3d)[axis]);
#endif
			}
			break;
		case clip_box::handle_trans_xyz:
			{
				g_box.m_trans += c3d - g_cursor.m_3d;
			}
			break;
		case clip_box::handle_rot_x:
		case clip_box::handle_rot_y:
		case clip_box::handle_rot_z:
			{
				int axis = g_box.m_active_handle - clip_box::handle_rot_x;
				g_box.m_rot[axis] += CG::degrees((c3d - g_cursor.m_3d)[axis]);
			}
			break;
		}

		// �}�E�X���݈ʒu���o���Ă���
		g_cursor.m_2d = c2d;
		g_cursor.m_3d = c3d;

		glutPostRedisplay();
	}
	else {
		g_eye = g_eye * 
			/*CG::mat4f::translate(-g_box.m_trans.x, -g_box.m_trans.y, -g_box.m_trans.z) * */
			CG::mat4f::rotate((c2d.x - g_cursor.m_2d.x) * 0.1f, CG::vec3f(0, 1, 0)) *
			CG::mat4f::rotate((c2d.y - g_cursor.m_2d.y) * 0.1f, CG::vec3f(1, 0, 0)) /* *
			CG::mat4f::translate(g_box.m_trans.x, g_box.m_trans.y, g_box.m_trans.z) */;
		fprintf(stderr, "eye (%f, %f, %f)\n", g_eye.x, g_eye.y, g_eye.z);
		g_cursor.m_2d = c2d;
		glutPostRedisplay();
	}
}
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, GLdouble(width) / height, 0.001, 1000);
	glMatrixMode(GL_MODELVIEW);
}
int _tmain(int argc, _TCHAR* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_ALPHA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("depthcamtest");

    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive_motion);

	glClearColor(0.25f, 0.25f, 0.25f, 0);
	glClearDepth( 1.0f );
	glShadeModel( GL_SMOOTH );
	glFrontFace( GL_CCW );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);

	update_camera();

	glutMainLoop();

	return 0;
}

