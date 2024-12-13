#include "plugin.hpp" // Your plugin.hpp must be included
#include <rack.hpp>   // Include only rack.hpp for Rack API

using namespace rack;

// Forward declare your module class and widget here if needed
struct MultiheadSequencer : engine::Module {
    enum ParamId {
        CV_1_PARAM,
        CV_3_PARAM,
        CV_5_PARAM,
        CV_2_PARAM,
        CV_7_PARAM,
        CV_4_PARAM,
        CV_6_PARAM,
        CV_8_PARAM,
        CV_9_PARAM,
        CV_11_PARAM,
        CV_13_PARAM,
        CV_10_PARAM,
        CV_15_PARAM,
        CV_12_PARAM,
        CV_14_PARAM,
        CV_16_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        CLOCK_1_INPUT,
        CLOCK_2_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUTPUT_PLAYHEAD_1_OUTPUT,
        OUTPUT_PLAYHEAD_2_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHT_1_LIGHT,
        LIGHT_3_LIGHT,
        LIGHT_5_LIGHT,
        LIGHT_2_LIGHT,
        LIGHT_7_LIGHT,
        LIGHT_4_LIGHT,
        LIGHT_6_LIGHT,
        LIGHT_8_LIGHT,
        LIGHT_9_LIGHT,
        LIGHT_11_LIGHT,
        LIGHT_13_LIGHT,
        LIGHT_10_LIGHT,
        LIGHT_15_LIGHT,
        LIGHT_12_LIGHT,
        LIGHT_14_LIGHT,
        LIGHT_16_LIGHT,
        LIGHTS_LEN
    };

    static const int NUM_STEPS = 16;
    static const int NUM_PLAYHEADS = 2;

    int stepParams[NUM_STEPS] = {
        CV_1_PARAM, CV_2_PARAM, CV_3_PARAM, CV_4_PARAM,
        CV_5_PARAM, CV_6_PARAM, CV_7_PARAM, CV_8_PARAM,
        CV_9_PARAM, CV_10_PARAM, CV_11_PARAM, CV_12_PARAM,
        CV_13_PARAM, CV_14_PARAM, CV_15_PARAM, CV_16_PARAM
    };

    int stepLights[NUM_STEPS] = {
        LIGHT_1_LIGHT, LIGHT_2_LIGHT, LIGHT_3_LIGHT, LIGHT_4_LIGHT,
        LIGHT_5_LIGHT, LIGHT_6_LIGHT, LIGHT_7_LIGHT, LIGHT_8_LIGHT,
        LIGHT_9_LIGHT, LIGHT_10_LIGHT, LIGHT_11_LIGHT, LIGHT_12_LIGHT,
        LIGHT_13_LIGHT, LIGHT_14_LIGHT, LIGHT_15_LIGHT, LIGHT_16_LIGHT
    };

    int stepIndex[NUM_PLAYHEADS];
    dsp::SchmittTrigger clockTrigger[NUM_PLAYHEADS];

    MultiheadSequencer() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        for (int i = 0; i < PARAMS_LEN; i++) {
            configParam(i, 0.f, 1.f, 0.f, "");
        }
        configInput(CLOCK_1_INPUT, "");
        configInput(CLOCK_2_INPUT, "");
        configOutput(OUTPUT_PLAYHEAD_1_OUTPUT, "");
        configOutput(OUTPUT_PLAYHEAD_2_OUTPUT, "");

        for (int i = 0; i < NUM_PLAYHEADS; i++) {
            stepIndex[i] = 0;
        }
    }

    void process(const ProcessArgs& args) override {
        for (int i = 0; i < NUM_PLAYHEADS; i++) {
            float clockIn = inputs[CLOCK_1_INPUT + i].getVoltage();
            if (clockTrigger[i].process(rescale(clockIn, 0.1f, 2.f, 0.f, 1.f))) {
                stepIndex[i] = (stepIndex[i] + 1) % NUM_STEPS;
            }

            float stepValue = params[stepParams[stepIndex[i]]].getValue();
            outputs[OUTPUT_PLAYHEAD_1_OUTPUT + i].setVoltage(stepValue * 10.f);
        }

        for (int s = 0; s < NUM_STEPS; s++) {
            float brightness = (s == stepIndex[0]) ? 1.f : 0.f;
            lights[stepLights[s]].setBrightness(brightness);
        }
    }
};

struct MultiheadSequencerWidget : app::ModuleWidget {
    MultiheadSequencerWidget(MultiheadSequencer* module) {
        setModule(module);
        // Use pluginInstance and asset::plugin to load your panel
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MultiheadSequencer.svg")));

        // Add screws
        addChild(createWidget<componentlibrary::ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<componentlibrary::ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<componentlibrary::ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<componentlibrary::ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        // Add all knobs, inputs, outputs, and lights
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(22.588, 54.014)), module, MultiheadSequencer::CV_1_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(77.562, 53.552)), module, MultiheadSequencer::CV_3_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(131.199, 53.19)), module, MultiheadSequencer::CV_5_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(49.711, 54.014)), module, MultiheadSequencer::CV_2_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(186.173, 52.728)), module, MultiheadSequencer::CV_7_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(104.685, 53.552)), module, MultiheadSequencer::CV_4_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(158.323, 53.19)), module, MultiheadSequencer::CV_6_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(213.297, 52.728)), module, MultiheadSequencer::CV_8_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(241.223, 52.973)), module, MultiheadSequencer::CV_9_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(296.196, 52.511)), module, MultiheadSequencer::CV_11_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(349.834, 52.149)), module, MultiheadSequencer::CV_13_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(268.346, 52.973)), module, MultiheadSequencer::CV_10_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(404.808, 51.687)), module, MultiheadSequencer::CV_15_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(323.32, 52.511)), module, MultiheadSequencer::CV_12_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(376.958, 52.149)), module, MultiheadSequencer::CV_14_PARAM));
        addParam(createParamCentered<componentlibrary::RoundBlackKnob>(mm2px(Vec(431.931, 51.687)), module, MultiheadSequencer::CV_16_PARAM));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.379, 11.353)), module, MultiheadSequencer::CLOCK_1_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.379, 23.47)), module, MultiheadSequencer::CLOCK_2_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(473.651, 83.488)), module, MultiheadSequencer::OUTPUT_PLAYHEAD_1_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(473.651, 97.954)), module, MultiheadSequencer::OUTPUT_PLAYHEAD_2_OUTPUT));

        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.083, 75.668)), module, MultiheadSequencer::LIGHT_1_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(77.057, 75.206)), module, MultiheadSequencer::LIGHT_3_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(130.695, 74.844)), module, MultiheadSequencer::LIGHT_5_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(49.207, 75.668)), module, MultiheadSequencer::LIGHT_2_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(185.669, 74.382)), module, MultiheadSequencer::LIGHT_7_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(104.181, 75.206)), module, MultiheadSequencer::LIGHT_4_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(157.818, 74.844)), module, MultiheadSequencer::LIGHT_6_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(212.792, 74.382)), module, MultiheadSequencer::LIGHT_8_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(240.718, 74.627)), module, MultiheadSequencer::LIGHT_9_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(295.692, 74.165)), module, MultiheadSequencer::LIGHT_11_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(349.33, 73.803)), module, MultiheadSequencer::LIGHT_13_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(267.842, 74.627)), module, MultiheadSequencer::LIGHT_10_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(404.303, 73.341)), module, MultiheadSequencer::LIGHT_15_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(322.816, 74.165)), module, MultiheadSequencer::LIGHT_12_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(376.453, 73.803)), module, MultiheadSequencer::LIGHT_14_LIGHT));
        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(431.427, 73.341)), module, MultiheadSequencer::LIGHT_16_LIGHT));
    }
};

Model* modelMultiheadSequencer = createModel<MultiheadSequencer, MultiheadSequencerWidget>("MultiheadSequencer");
