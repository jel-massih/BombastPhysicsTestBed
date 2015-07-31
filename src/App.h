#ifndef APP_H
#define APP_H

class App
{
public:
	virtual const char* GetTitle();

	virtual void Initialize();
	virtual void Shutdown();

	virtual void SetCamera();

	virtual void Render();
	virtual void Update();

	virtual void KeyEvent(int key, int scanCode, int action, int mods);
	virtual void CharEvent(unsigned int code);
	virtual void MouseClick(int button, int action, int mods);
	virtual void MouseDrag(int x, int y);

	void RenderText(float x, float y, const char* text);

	virtual void WindowResized(int width, int height);

protected:
	int m_width, m_height;
};

#endif