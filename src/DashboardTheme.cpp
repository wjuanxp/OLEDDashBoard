#include "DashboardTheme.h"

namespace OledDashboard {

Theme themeFor(ThemeId id) {
    Theme t;  // defaults are the Professional theme
    t = Theme();  // ensure value initialisation from defaults
    switch (id) {
        case ThemeId::kMinimal:
            t.showDividers = false;
            t.showGraphBorder = false;
            t.graphStyle = GraphStyle::kLine;
            t.graphFade = false;
            break;

        case ThemeId::kIndustrial:
            t.graphStyle = GraphStyle::kBars;
            t.graphFade = false;
            t.showDividers = true;
            t.showGraphBorder = true;
            break;

        case ThemeId::kDark:
            t.graphStyle = GraphStyle::kFilled;
            t.graphFade = true;
            t.showDividers = true;
            t.padding = 1;
            break;

        case ThemeId::kModern:
            t.graphStyle = GraphStyle::kDots;
            t.graphFade = false;
            t.showDividers = false;
            t.showGraphBorder = false;
            break;

        case ThemeId::kProfessional:
        default:
            break;  // already the defaults
    }
    return t;
}

}  // namespace OledDashboard
