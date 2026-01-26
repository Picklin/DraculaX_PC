#include "Intro.h"
#include <functional>
#include <fstream>
#include <sstream>
using namespace std;

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
    cinematic->dialogueTxt = Text::CreateDialogueText(shader);
    cinematic->init(shader);
    return cinematic;
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
