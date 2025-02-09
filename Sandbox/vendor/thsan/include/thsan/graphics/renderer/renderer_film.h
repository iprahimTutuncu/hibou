#pragma once
#include "renderer.h"
#include <queue>
#include <memory>
#include <stack>
#include <unordered_set>
#include <string>
#include <map>

namespace Thsan::UI
{
	class Gui;
}

namespace Thsan {
	class RenderTarget;
	class Framebuffer;
	class RenderContext;
	class RenderStates;
	class RenderStates3D;
	class Drawable;
	class Shader;
	class Modifier;
	class Mesh;
	class View;
	class Game;
	class Texture2D;
	class ComputeShader;
	struct WindowOption;
	struct LightProperties;

	class THSAN_API RendererFilm : public Renderer {
	public:
		RendererFilm() = default;
		~RendererFilm() = default;

		virtual bool init(WindowOption windowOptions) = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setView(std::shared_ptr<View> view) = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void display() = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual void addLight(LightProperties* light) = 0;

		virtual bool importFrame(std::string frame_path, int index) = 0;
		virtual bool exportFrame(std::string frame_path) = 0;

		virtual void setKeyframe(int frameIndex) = 0; 
		virtual void removeKeyframe(int frameIndex) = 0;

	private:
		static RendererFilm* create();

		friend class Game;
	};


	class RendererFilmImpl : public RendererFilm {
	public:
		RendererFilmImpl() = default;
		~RendererFilmImpl() = default;

		bool init(WindowOption windowOptions) override;
		void close() override;
		void clear() override;
		void setView(std::shared_ptr<View> view) override;
		void setViewport(int x, int y, int w, int h) override;
		void setDefaultViewport(int x, int y, int w, int h) override;
		void display() override;

		void addLight(LightProperties* light) override;

		bool importFrame(std::string frame_path, int index) override;
		bool exportFrame(std::string frame_path) override;

		void show() override;
		void hide() override;

		void setKeyframe(int frameIndex) override;
		void removeKeyframe(int frameIndex) override;


	private:

		struct Frame 
		{
			std::shared_ptr<Texture2D> diffuse;
			std::shared_ptr<Texture2D> positionMap;
			std::shared_ptr<Texture2D> rayTracedResult;
		};

		int x{ 0 }, y{ 0 }, w{ 1280 }, h{ 768 };
		bool isShowEnable{ true };

		std::map<int, Frame> keyframes;
		std::vector<Frame> frames;     

		int currentFrame{ -1 };         
		int totalFrames{ 0 };          

		std::shared_ptr<Mesh> screenQuad;
		std::shared_ptr<View> screenView;

		std::shared_ptr<RenderStates> defaultState;
		std::shared_ptr<Shader> defaultShader = nullptr;

		std::shared_ptr<ComputeShader> rayTracingShader = nullptr; // Screen-space ray tracing

		void applyRayTracing(Frame& frame);
	};

}