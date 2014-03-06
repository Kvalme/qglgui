/*
 * Copyright (c) 2014, Denis Biryukov <denis.birukov@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Denis Biryukov <denis.birukov@gmail.com> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Denis Biryukov <denis.birukov@gmail.com> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once
#include <memory>
#include <functional>

class QWidget;

namespace QGL
{

class GuiWindowFactory;

/**
 * Main class for GlGui. Handles GUI initialization.
 * Can work in both - multi and single thread enviroment.
 */
class GlGui
{
public:
	enum class THREADING_MODE
	{
		SINGLE,
		MULTI
	};

	/**
	 * Create GlGui instance to operate in specified threading mode.
	 * Single thread require to call update() method manually
	 *
	 * @param mode threading mode
	 * @return pointer to created gui
	 */
	static std::shared_ptr<GlGui> Create(THREADING_MODE mode);

	/**
	 * Registers window factory.
	 * Each call to CreateWindow will try to create window via provided factory
	 *
	 * @param factory pointer to the factory
	 */
	virtual void RegisterWindowFactory(std::function<QWidget*(const std::string &name)> factory) = 0;

	/**
	 * Creates window by provided name.
	 * GuiWindowFactory will be called to create window.
	 *
	 * @param name window name
	 */
	virtual void CreateWindow(const std::string &name) = 0;

protected:
	GlGui();
	GlGui(GlGui &);
	virtual ~GlGui();
};

}

