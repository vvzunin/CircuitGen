CREATE TABLE "Mutation" (
    "id" INTEGER NOT NULL,
    "mut_type" INTEGER NOT NULL,
    "mut_chance" DOUBLE PRECISION NOT NULL,
    "type_of_exchange" INTEGER NOT NULL,
    "ratio_in_table" DOUBLE PRECISION NOT NULL
);
ALTER TABLE "Mutation" ADD PRIMARY KEY("id");

CREATE TABLE "TypeOfParentSelection" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "TypeOfParentSelection" ADD PRIMARY KEY("id");

CREATE TABLE "SelectionType" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "SelectionType" ADD PRIMARY KEY("id");

CREATE TABLE "GenerationMethods" (
    "id" INTEGER NOT NULL,
    "randLevel" INTEGER NOT NULL,
    "randLevelExperimental" BIGINT NOT NULL,
    "fromRandomTruthTable" INTEGER NOT NULL,
    "numOperation" INTEGER NOT NULL,
    "genetic" INTEGER NOT NULL,
    "comparison" INTEGER NOT NULL,
    "subtractor" INTEGER NOT NULL,
    "summator" INTEGER NOT NULL,
    "defaults" INTEGER NOT NULL
);
ALTER TABLE "GenerationMethods" ADD PRIMARY KEY("id");

CREATE TABLE "Playback" (
    "id" INTEGER NOT NULL,
    "selection_type_parent" INTEGER NOT NULL,
    "tour_size" INTEGER NOT NULL,
    "playback_type" INTEGER NOT NULL,
    "ref_points" INTEGER NOT NULL,
    "mask_prob" DOUBLE PRECISION NOT NULL,
    "rec_num" INTEGER NOT NULL
);
ALTER TABLE "Playback" ADD PRIMARY KEY("id");

CREATE TABLE "Summator" (
    "id" INTEGER NOT NULL,
    "overflowIn" BOOLEAN NOT NULL,
    "overflowOut" BOOLEAN NOT NULL,
    "minus" BOOLEAN NOT NULL
);
ALTER TABLE "Summator" ADD PRIMARY KEY("id");

CREATE TABLE "TypeOfExchange" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "TypeOfExchange" ADD PRIMARY KEY("id");

CREATE TABLE "Selection" (
    "id" INTEGER NOT NULL,
    "selection_type" INTEGER NOT NULL,
    "surv_num" INTEGER NOT NULL
);
ALTER TABLE "Selection" ADD PRIMARY KEY("id");

CREATE TABLE "Genetic" (
    "id" INTEGER NOT NULL,
    "chromosome_type" VARCHAR(80) NOT NULL,
    "playback" INTEGER NOT NULL,
    "mutation" INTEGER NOT NULL,
    "selection" INTEGER NOT NULL,
    "population_size" INTEGER NOT NULL,
    "cycles" INTEGER NOT NULL,
    "out_ratio" INTEGER NOT NULL
);
ALTER TABLE "Genetic" ADD PRIMARY KEY("id");

CREATE TABLE "TypeOfMutation" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "TypeOfMutation" ADD PRIMARY KEY("id");

CREATE TABLE "TypeOfPlayback" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "TypeOfPlayback" ADD PRIMARY KEY("id");

ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_summator_foreign" FOREIGN KEY("summator") REFERENCES "Summator"("id");
ALTER TABLE "Genetic" ADD CONSTRAINT "genetic_playback_foreign" FOREIGN KEY("playback") REFERENCES "Playback"("id");
ALTER TABLE "Mutation" ADD CONSTRAINT "mutation_mut_type_foreign" FOREIGN KEY("mut_type") REFERENCES "TypeOfMutation"("id");
ALTER TABLE "Genetic" ADD CONSTRAINT "genetic_mutation_foreign" FOREIGN KEY("mutation") REFERENCES "Mutation"("id");
ALTER TABLE "Playback" ADD CONSTRAINT "playback_playback_type_foreign" FOREIGN KEY("playback_type") REFERENCES "TypeOfPlayback"("id");
ALTER TABLE "Playback" ADD CONSTRAINT "playback_selection_type_parent_foreign" FOREIGN KEY("selection_type_parent") REFERENCES "TypeOfParentSelection"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_genetic_foreign" FOREIGN KEY("genetic") REFERENCES "Genetic"("id");
ALTER TABLE "Selection" ADD CONSTRAINT "selection_selection_type_foreign" FOREIGN KEY("selection_type") REFERENCES "SelectionType"("id");
ALTER TABLE "Mutation" ADD CONSTRAINT "mutation_type_of_exchange_foreign" FOREIGN KEY("type_of_exchange") REFERENCES "TypeOfExchange"("id");
ALTER TABLE "Genetic" ADD CONSTRAINT "genetic_selection_foreign" FOREIGN KEY("selection") REFERENCES "Selection"("id");

CREATE TABLE "Subtractor" (
    "id" INTEGER NOT NULL,
    "overflowIn" BOOLEAN NOT NULL,
    "overflowOut" BOOLEAN NOT NULL,
    "sub" BOOLEAN NOT NULL
);
ALTER TABLE "Subtractor" ADD PRIMARY KEY("id");

CREATE TABLE "InputData" (
    "id" INTEGER NOT NULL,
    "generationParameters" INTEGER NOT NULL,
    "circuitGenParameters" INTEGER NOT NULL,
    "seed" INTEGER NOT NULL
);
ALTER TABLE "InputData" ADD PRIMARY KEY("id");

CREATE TABLE "RandLevel" (
    "id" INTEGER NOT NULL,
    "min_level" INTEGER NOT NULL,
    "max_level" INTEGER NOT NULL,
    "min_elem" INTEGER NOT NULL,
    "max_elen" INTEGER NOT NULL
);
ALTER TABLE "RandLevel" ADD PRIMARY KEY("id");

CREATE TABLE "TypeAbc" (
    "id" INTEGER NOT NULL,
    "name" TEXT NOT NULL,
    "description" TEXT NOT NULL
);
ALTER TABLE "TypeAbc" ADD PRIMARY KEY("id");

CREATE TABLE "ParametersResult" (
    "id" INTEGER NOT NULL,
    "area" DOUBLE PRECISION NOT NULL,
    "delay" DOUBLE PRECISION NOT NULL,
    "edge" INTEGER NOT NULL,
    "fileRead" BOOLEAN NOT NULL,
    "inputs" INTEGER NOT NULL,
    "lat" INTEGER NOT NULL,
    "lev" INTEGER NOT NULL,
    "nd" INTEGER NOT NULL,
    "outputs" INTEGER NOT NULL,
    "type_abc" INTEGER NOT NULL,
    "abc_verilog" TEXT NOT NULL,
    "firtll" TEXT NOT NULL,
    "bench" TEXT NOT NULL,
    "lib" TEXT NOT NULL
);
ALTER TABLE "ParametersResult" ADD PRIMARY KEY("id");

CREATE TABLE "FromRandomTruthTable" (
    "id" INTEGER NOT NULL,
    "CNFF" BOOLEAN NOT NULL,
    "CNFT" BOOLEAN NOT NULL,
    "Zhegalkin" BOOLEAN NOT NULL
);
ALTER TABLE "FromRandomTruthTable" ADD PRIMARY KEY("id");

CREATE TABLE "GeneratorResult" (
    "id" INTEGER NOT NULL,
    "numInputs" INTEGER NOT NULL,
    "numOutputs" INTEGER NOT NULL,
    "maxLevel" INTEGER NOT NULL,
    "numEdges" INTEGER NOT NULL,
    "numGates" INTEGER NOT NULL,
    "and" INTEGER NOT NULL,
    "buf" INTEGER NOT NULL,
    "nand" INTEGER NOT NULL,
    "nor" INTEGER NOT NULL,
    "or" INTEGER NOT NULL,
    "xnor" INTEGER NOT NULL,
    "xor" INTEGER NOT NULL,
    "numEdgesOfEachType" JSON NOT NULL,
    "path_verilog" TEXT NOT NULL,
    "graphml_classic" TEXT NOT NULL,
    "graphml-pseudo_abc_d" TEXT NOT NULL,
    "graphml_open_abc_d" TEXT NOT NULL
);
ALTER TABLE "GeneratorResult" ADD PRIMARY KEY("id");

CREATE TABLE "GenerationParameters" (
    "id" INTEGER NOT NULL,
    "min_in" INTEGER NOT NULL,
    "max_in" INTEGER NOT NULL,
    "min_out" INTEGER NOT NULL,
    "max_out" INTEGER NOT NULL,
    "make_graphml_classic" BOOLEAN NOT NULL,
    "make_graphml-pseudo_abc_d" BOOLEAN NOT NULL,
    "make_graphml_open_abc_d" BOOLEAN NOT NULL,
    "type_of_generation" INTEGER NOT NULL
);
ALTER TABLE "GenerationParameters" ADD PRIMARY KEY("id");

CREATE TABLE "CircuitGenParameters" (
    "id" INTEGER NOT NULL,
    "make_resyn2" BOOLEAN NOT NULL,
    "make_balance" BOOLEAN NOT NULL,
    "to_bench" BOOLEAN NOT NULL,
    "to_firrtl" BOOLEAN NOT NULL,
    "lib_name" TEXT NOT NULL
);
ALTER TABLE "CircuitGenParameters" ADD PRIMARY KEY("id");

CREATE TABLE "Comparison" (
    "id" INTEGER NOT NULL,
    "equal" BOOLEAN NOT NULL,
    "less" BOOLEAN NOT NULL,
    "more" BOOLEAN NOT NULL
);
ALTER TABLE "Comparison" ADD PRIMARY KEY("id");

CREATE TABLE "NumOperation" (
    "id" INTEGER NOT NULL,
    "leave_empty_out" BOOLEAN NOT NULL
);
ALTER TABLE "NumOperation" ADD PRIMARY KEY("id");

CREATE TABLE "RandLevelExperimental" (
    "id" INTEGER NOT NULL,
    "min_level" INTEGER NOT NULL,
    "max_level" INTEGER NOT NULL,
    "min_elem" INTEGER NOT NULL,
    "max_elen" INTEGER NOT NULL,
    "gates_inputs_info" JSON NOT NULL
);
ALTER TABLE "RandLevelExperimental" ADD PRIMARY KEY("id");

CREATE TABLE "Circuit" (
    "id" INTEGER NOT NULL,
    "parameters" INTEGER NOT NULL,
    "generator" INTEGER NOT NULL
);
ALTER TABLE "Circuit" ADD PRIMARY KEY("id");

CREATE TABLE "Results" (
    "id" INTEGER NOT NULL,
    "inputData" INTEGER NOT NULL,
    "CircuitId" INTEGER NOT NULL
);
ALTER TABLE "Results" ADD PRIMARY KEY("id");

ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_numoperation_foreign" FOREIGN KEY("numOperation") REFERENCES "NumOperation"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_fromrandomtruthtable_foreign" FOREIGN KEY("fromRandomTruthTable") REFERENCES "FromRandomTruthTable"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_comparison_foreign" FOREIGN KEY("comparison") REFERENCES "Comparison"("id");
ALTER TABLE "Circuit" ADD CONSTRAINT "circuit_parameters_foreign" FOREIGN KEY("parameters") REFERENCES "ParametersResult"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_subtractor_foreign" FOREIGN KEY("subtractor") REFERENCES "Subtractor"("id");
ALTER TABLE "Circuit" ADD CONSTRAINT "circuit_generator_foreign" FOREIGN KEY("generator") REFERENCES "GeneratorResult"("id");
ALTER TABLE "GenerationParameters" ADD CONSTRAINT "generationparameters_type_of_generation_foreign" FOREIGN KEY("type_of_generation") REFERENCES "GenerationMethods"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_randlevel_foreign" FOREIGN KEY("randLevel") REFERENCES "RandLevel"("id");
ALTER TABLE "GenerationMethods" ADD CONSTRAINT "generationmethods_randlevelexperimental_foreign" FOREIGN KEY("randLevelExperimental") REFERENCES "RandLevelExperimental"("id");
ALTER TABLE "Results" ADD CONSTRAINT "results_circuitid_foreign" FOREIGN KEY("CircuitId") REFERENCES "Circuit"("id");
ALTER TABLE "InputData" ADD CONSTRAINT "inputdata_circuitgenparameters_foreign" FOREIGN KEY("circuitGenParameters") REFERENCES "CircuitGenParameters"("id");
ALTER TABLE "Results" ADD CONSTRAINT "results_inputdata_foreign" FOREIGN KEY("inputData") REFERENCES "InputData"("id");
ALTER TABLE "InputData" ADD CONSTRAINT "inputdata_generationparameters_foreign" FOREIGN KEY("generationParameters") REFERENCES "GenerationParameters"("id");
ALTER TABLE "ParametersResult" ADD CONSTRAINT "parametersresult_type_abc_foreign" FOREIGN KEY("type_abc") REFERENCES "TypeAbc"("id");
