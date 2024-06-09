import ToggleSwitchFilter from "./ToggleSwitch";

const GraphML = ({ updateState, state }) => {

    const changeMakeGraphmlClassic = (val) => {
        updateState('make_graphml_classic', val);
    }

    const changeMakeGraphmlOpenABC_D = (val) => {
        updateState('make_graphml_open_abc_d', val);
    }

    const changeMakeGraphmlPseudoOpenABC_D = (val) => {
        updateState('make_graphml_pseudo_abc_d', val);
    }

    return (
        <div className="add__truth">
            <div className='add__truth-name'>Создать классический graphML</div><ToggleSwitchFilter isChecked={state.make_graphml_classic} changeParentState={(upd) => changeMakeGraphmlClassic(upd)} />
            <div className='add__truth-name'>Создать graphML для OpenABC-D</div><ToggleSwitchFilter isChecked={state.make_graphml_open_abc_d} changeParentState={(upd) => changeMakeGraphmlOpenABC_D(upd)} />
            <div className='add__truth-name'>Создать урезанный graphML для OpenABC-D</div><ToggleSwitchFilter isChecked={state.make_graphml_pseudo_abc_d} changeParentState={(upd) => changeMakeGraphmlPseudoOpenABC_D(upd)} />
        </div>
    )
}

export default GraphML;