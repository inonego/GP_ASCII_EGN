#include "Render.h"

int Render::currentBufferIndex = 0;

int Render::width = 192;
int Render::height = 48;

FrameBuffer* Render::buffer[MAX_BUFFER_COUNT];

void Render::Initialize() {
    for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
        buffer[i] = new FrameBuffer(width, height);
    }
}

Render::~Render() {
    for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
        delete buffer[i];
    }
}

void Render::Update()
{
	ShowBuffer();
	ClearBuffer();
	FlipBuffer();
}

void Render::ShowBuffer() {
    buffer[currentBufferIndex]->Show();
}

void Render::FlipBuffer() {
    currentBufferIndex = (currentBufferIndex + 1) % MAX_BUFFER_COUNT;
}

void Render::ClearBuffer() {
    buffer[currentBufferIndex]->Clear();
}