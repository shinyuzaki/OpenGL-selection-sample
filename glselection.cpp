#include "stdafx.h"


// クリップボックス
struct clip_box
{
	// ピックハンドルインデックス
	enum handle {
		handle_none = 0,	//!< ピックなし
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
	// 座標軸インデックス
	enum axis {
		axis_x_pos = 0,
		axis_x_neg,
		axis_y_pos,
		axis_y_neg,
		axis_z_pos,
		axis_z_neg,
		axis_cnt,
	};

	// 描画色設定.
	void setup_color(
		handle condition	//!< [in] m_active_handleと等しい場合ハイライト色
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
	
	// 箱を描画する.
	void draw_cube(
		const float* box	//!< [in] enum axis で参照する配列.
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
	// マニピュレータハンドル描画.
	// 描画コード内にName stackを仕込んでます.
	void draw_handle() const
	{
		// マニピュレータハンドルを描画する際はdepth testを切る
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

	// クリップボックスとマニピュレータハンドル描画
	void draw(
		bool use_handle	//!< [in] trueの場合マニピュレータハンドル描画
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

	// ピック処理
	// @return ヒットしたマニピュレータ
	handle hit
		(int x,			//!< [in] マウス座標X
		 int y,			//!< [in] マウス座標Y
		 int area,		//!< [in] ピック範囲(ウィンドウ座標系)
		 double& depth	//!< [out] ピック位置デプス
		 ) const
	{
		depth = 0;

		// 現在のビューポート/投影行列を得る
		GLint viewport[4];
		CG::mat4d projection_matrix;
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

		// PickMatrixに現在の投影行列を乗算する
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();		// スタックを一段上げて現在の状態を退避
		glLoadIdentity();
		// PickMatrix
		gluPickMatrix(x, viewport[3] - y, area, area, viewport);
		// 現在の投影行列
		glMultMatrixd(projection_matrix);
		glMatrixMode(GL_MODELVIEW);

		// selection drawの準備
		GLuint pick_buffer[64];
		glSelectBuffer(sizeof(pick_buffer)/sizeof(*pick_buffer), pick_buffer);
		glRenderMode(GL_SELECT);
		glInitNames();

		// 描画は通常通り
		draw(true);

		// 退避した投影行列を復帰
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		// 通常描画に戻すとヒット数が返ってくる
		int hits = glRenderMode(GL_RENDER);

		// selection 結果解析
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

				// 一番手前の選択を得る
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

	// コンストラクタ
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

	float m_handle_cube_size;			//!< translate キューブハンドルのサイズ
	float m_handle_axis_length;			//!< 軸ごとハンドルの長さ
	CG::vec4f m_cube_color;				//!< クリップボックスの色
	CG::vec4f m_handle_color;			//!< マニピュレータハンドルの通常色
	CG::vec4f m_handle_highlight_color;	//!< マニピュレータハンドルのハイライト色
	handle	  m_active_handle;			//!< 現在アクティブなマニピュレータハンドル

	CG::vec3f m_trans;					//!< クリップボックス位置
	CG::vec3f m_rot;					//!< クリップボックスオイラー角(degree)
	float	  m_box[6];					//!< クリップボックス位置からクリップ面への距離
};

// マウスカーソル
struct cursor
{
	// 2Dから3Dへマウス座標を変換する
	// @return 3Dマウス座標
	CG::vec3f unproject
		(int x,			//!< [in] マウス座標X
		 int y,			//!< [in] マウス座標Y
		 double depth	//!< [in] ピック位置デプス
		 ) const
	{
		// 現在のビューポート/投影行列を得る
		GLint viewport[4];
		CG::mat4d projection_matrix;
		CG::mat4d modelview_matrix;

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);

		// それらを使用して2Dマウス座標を3Dに変換する
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
		// マウス初期位置を覚えておく
		g_cursor.m_2d.x = x;
		g_cursor.m_2d.y = y;
		g_cursor.m_3d = g_cursor.unproject(x, y, g_cursor.m_depth);
	}
}
void passive_motion(int x, int y)
{
	// ただマウスが動いてる時にマニピュレータピックを行なう
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
	// マウス現在位置
	CG::vec2<int> c2d(x, y);

	// passive_motion()でマニピュレータにマウス座標がヒットしてる時だけ処理を行う
	if (g_box.m_active_handle)
	{
		// マウス現在位置
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
#if 0	// こっちだと移動
				g_box.m_trans[axis] += (c3d - g_cursor.m_3d)[axis];
#else	// こっちだと回転(本来はキーモディファイア(Ctrl key)とかで切り替え
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

		// マウス現在位置を覚えておく
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

