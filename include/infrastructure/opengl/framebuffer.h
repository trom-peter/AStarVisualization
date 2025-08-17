#pragma once
#include "infrastructure/opengl/base_renderer.h"
#include <cstdint>

struct FrameBuffer {
	FrameBuffer(const uint32_t width, const uint32_t height) {
		this->width = width;
		this->height = height;

		glGenFramebuffers(1, &framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

		glGenTextures(1, &colorTextureId);
		glBindTexture(GL_TEXTURE_2D, colorTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual ~FrameBuffer() {
		glDeleteFramebuffers(1, &framebufferId);
		glDeleteTextures(1, &colorTextureId);
		glDeleteRenderbuffers(1, &renderbufferId);
	}

	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
	}

	void unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void resize(const uint32_t width, const uint32_t height) {
		this->width = width;
		this->height = height;

		bind();

		glBindTexture(GL_TEXTURE_2D, colorTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);

		glDeleteRenderbuffers(1, &renderbufferId);

		glGenRenderbuffers(1, &renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		unbind();
	}

	float width, height;
	GLuint framebufferId;
	GLuint colorTextureId;
	GLuint renderbufferId;
};