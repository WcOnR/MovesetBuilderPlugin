## Moveset Builder
**Moveset Builder** - is plugin for unreal engine 4.26. The plugin allows customize moveset for your character.
### Main classes
`MovesetComponent` - Component responsible for manage *AttackMotion* objects of the single moveset. 
`AttackMotion` - Main class for building attack motion sequence. You may override *AttackInput* function for set up next *AttackMotion* depending on the value of the attack type.
`AttackInputTypeDataAsset` - Input type of attack, useful for define several attack types. For example: attack, strong attack and charge attack (holding button).
`ReadyToInterruptMotion` - *AnimNotify* which telling *MovesetComponent* about opportunity to interrupt the current *AttackMotion*. 
`MovesetComponentProxy` - Proxy interface for providing *ReadyToInterruptMotion* the correct *MovesetComponent*.
### Tips
- Don't forget set *SkeletalMeshComponent* and *HeadAttackMotionClass* for *MovesetComponent*.
- If Animation montage didn't play. Try to check did you set up slot for *AnimationBlueprint*?