#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "Cinematic.h"
#include "Game.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>
#include <codecvt>
#include <locale>

Cinematic::~Cinematic()
{
    for (TexturedQuad* q : cinematicQuads)
    {
        q->free();
        delete q;
    }
    for (Sprite* s : cinematicSprites)
    {
        s->free();
        delete s;
	}
    if (blackScreen)
    {
        blackScreen->free();
        delete blackScreen;
    }
    if (dialogueTxt)
    {
        dialogueTxt->free();
        delete dialogueTxt;
	}
}

void Cinematic::update(int deltaTime)
{
    timeElapsed += deltaTime / 1000.f;
    if (!script.empty())
    {
        if ((timeElapsed >= script.front().time) && (timeElapsed <= (script.front().time + script.front().duration)))
        {
            renderSubtitles = true;
        }
        else if (timeElapsed > (script.front().time + script.front().duration))
        {
            renderSubtitles = false;
            script.pop();
        }
    }
    if (!film.empty())
    {
        if ((timeElapsed >= film.front().time) && (timeElapsed <= (film.front().time + film.front().duration)))
        {
            film.front().bg->update(deltaTime);
            filmUpdate(deltaTime);
            float alpha = film.front(). alpha;
            if (alpha < 1.f)
            {
                alpha += deltaTime / 1000.f;
                blackScreen->setAlpha(1 - alpha);
                film.front().alpha = alpha;
            }
            renderBg = true;
        }
        else if (timeElapsed > (film.front().time + film.front().duration))
        {
            renderBg = false;
			film.front().bg->free();
			delete film.front().bg;
            film.pop();
        }
    }
    if (Game::instance().getKey(GLFW_KEY_ENTER))
    {
        timeElapsed = endTime;
        Game::instance().keyReleased(GLFW_KEY_ENTER);
    }
}

bool Cinematic::ended() const
{
    return timeElapsed >= endTime;
}

void Cinematic::init(ShaderProgram& shader)
{
    this->shader = &shader;
    blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *TextureManager::instance().getTexture("pixel"), shader);
    blackScreen->setColor(glm::vec3(0));
    blackScreen->setAlpha(0.f);
    initChild();
    endTime = setEndTime();
    this->shader->setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f));
}

void Cinematic::loadScript(const string& scriptPath)
{
    wifstream file(scriptPath, ios::binary);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
    wstring lineRaw;
    while (getline(file, lineRaw))
    {
        if (lineRaw.empty()) continue;
        if (!lineRaw.empty() && lineRaw.back() == L'\r') {
            lineRaw.pop_back();
        }
        wstringstream ss(lineRaw);
        wstring segment;
        Line lineInfo;

        //Read subtitle appearance time
        if (getline(ss, segment, L'|'))
        {
            lineInfo.time = stof(segment);
        }
        //Read subtitle duration
        if (getline(ss, segment, L'|'))
        {
            lineInfo.duration = stof(segment);
        }
        if (getline(ss, segment)) {
            size_t pos = 0;
            while ((pos = segment.find(L"\\n", pos)) != wstring::npos) {
                segment.replace(pos, 2, L"\n");
                pos += 1;
            }
            lineInfo.line = segment;
        }
        script.push(lineInfo);
    }
    file.close();
	dialogueTxt = Text::CreateDialogueText(*shader);
}
