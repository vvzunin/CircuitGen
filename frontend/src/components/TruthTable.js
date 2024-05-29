import ToggleSwitchFilter from "./ToggleSwitch";

const TruthTable = ({updateState, state}) => {

    const changeLimit = (limit) => {
      // updateState('limit', limit)
    }
  
    const changeCNFF = (CNFF) => {
      if (!CNFF) {
        if(state.CNFT || state.Zhegalkin) {
          updateState('CNFF', false);
        } else {
          updateState('CNFT', true);
          updateState('CNFF', false);
        }
      } else {
        updateState('CNFF', true);
      }
    }
  
    const changeCNFT = (CNFT) => {
      if (!CNFT) {
        if(state.CNFF || state.Zhegalkin) {
          updateState('CNFT', false);
        } else {
          updateState('CNFF', true);
          updateState('CNFT', false);
        }
      } else {
        updateState('CNFT', true);
      }
    }
  
    const changeZhegalkin = (Zhegalkin) => {
      if (!Zhegalkin) {
        if(state.CNFF || state.CNFT) {
          updateState('Zhegalkin', false);
        } else {
          updateState('CNFT', true);
          updateState('Zhegalkin', false);
        }
      } else {
        updateState('Zhegalkin', true);
      }
    }
  
    return (
      <div className="add__truth">
        {/* <div className='add__truth-name'>Ограничение генерации</div><ToggleSwitchFilter changeParentState={(limit) => changeLimit(limit)}/> */}
        <div className='add__truth-name'>CNFF</div><ToggleSwitchFilter isChecked={state.CNFF} changeParentState={(CNFF) => changeCNFF(CNFF)}/>
        <div className='add__truth-name'>CNFT</div><ToggleSwitchFilter isChecked={state.CNFT} changeParentState={(CNFT) => changeCNFT(CNFT)}/>
        <div className='add__truth-name'>Zhegalkin</div><ToggleSwitchFilter isChecked={state.Zhegalkin} changeParentState={(Zhegalkin) => changeZhegalkin(Zhegalkin)}/>
      </div>
    )
}

export default TruthTable;