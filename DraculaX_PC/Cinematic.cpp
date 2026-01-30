#include "Intro.h"
#include "Game.h"
#include "TextureManager.h"
#include <functional>
#include <fstream>
#include <sstream>

using CinematicCreator = function<Cinematic* ()>;
static const CinematicCreator cinematicCreator[Cinematic::COUNT] =
{
    []() {return new Intro(); }
};

Cinematic* Cinematic::createCinematic(ShaderProgram& shader, const string& scriptPath, int cinematicId)
{
    Cinematic* cinematic = cinematicCreator[cinematicId]();
    cinematic->dialogueTxt = Text::CreateDialogueText(shader);
    cinematic->init(shader);
    cinematic->loadScript(scriptPath);
    return cinematic;
}

Cinematic* Cinematic::createCinematic(ShaderProgram& shader, int cinematicId)
{
    Cinematic* cinematic = cinematicCreator[cinematicId]();
    cinematic->init(shader);
    return cinematic;
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
        }
        else if (timeElapsed > (film.front().time + film.front().duration))
        {
            renderBg = false;
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
    initChild();
    endTime = setEndTime();
    this->shader->setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f));
}

void Cinematic::loadScript(const string& scriptPath)
{
    wifstream file(scriptPath, ios::binary);
    file.imbue(locale("es_ES.UTF-8"));
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
}
